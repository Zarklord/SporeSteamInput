// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <steam/steam_api.h>

#include "GameModeListener.h"
#include "SteamInputCheat.h"
#include "SteamInputManager.h"
#include "Spore/App/cGameModeManager.h"

bool gSteamAPI_Init = false;

bool RealMain()
{
	//ManualBreakpoint();

	if (!SteamAPI_Init())
		return false;

	SteamInputManager::Initialize();

	return true;
}

void RealExit()
{
	SteamInputManager::Finalize();

	SteamAPI_Shutdown();
}

void OnEditorUndoPressed()
{
	auto editor = Editors::GetEditor();
	if (!editor) return;
	editor->Undo(true, true);
}

void OnEditorRedoPressed()
{
	auto editor = Editors::GetEditor();
	if (!editor) return;
	editor->Redo();
}

intrusive_ptr<App::UpdateMessageListener> sSteamAPIRunCallbacksUpdater;

void PostInitialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
	//ManualBreakpoint();

	SteamInputManager& steam_input_manager = GetSteamInputManager();

	steam_input_manager.OnPostInit();
	sSteamAPIRunCallbacksUpdater = App::AddUpdateFunction(SteamAPI_RunCallbacks);
	
	steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_UNDO, OnEditorUndoPressed);
	steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_REDO, OnEditorRedoPressed);

	CheatManager.AddCheat("steam_input_toggle", new SteamInputCheat(steam_input_manager));

	MessageManager.AddListener(new GameModeListener(steam_input_manager), App::OnModeEnterMessage::ID);
}

void Dispose()
{
	App::RemoveUpdateFunction(sSteamAPIRunCallbacksUpdater);
	sSteamAPIRunCallbacksUpdater = nullptr;
	// This method is called when the game is closing
}

void AttachDetours()
{
}

// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		gSteamAPI_Init = RealMain();

		if (!gSteamAPI_Init) break;
		
		ModAPI::AddPostInitFunction(PostInitialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
		if (!gSteamAPI_Init) break;
		RealExit();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

