// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <steam/steam_api.h>

#include "SteamInputCheat.h"
#include "SteamInputManager.h"
#include "UpdateLoop.h"

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
	
	steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_UNDO, OnEditorUndoPressed);
	steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_REDO, OnEditorRedoPressed);

	CheatManager.AddCheat("steam_input_toggle", new SteamInputCheat(steam_input_manager));
}

void Dispose()
{
	// This method is called when the game is closing
}

void OnEnterEditor(Editors::EditorRequest* request)
{
	GetSteamInputManager().SetActionSet(InputSet::EDITOR);
	//TODO determine what specific layer to apply.
}

static_detour(EditorRequestSubmit_detour, void(Editors::EditorRequest*))
{
	void detoured(Editors::EditorRequest* request)
	{
		OnEnterEditor(request);
		return original_function(request);
	}
};

member_detour(SubmitEditorRequest_detour, Simulator::cGameModeManager, void(Editors::EditorRequest*))
{
	void detoured(Editors::EditorRequest* request)
	{
		OnEnterEditor(request);
		return original_function(this, request);
	}
};

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	EditorRequestSubmit_detour::attach(GetAddress(Editors::EditorRequest, Submit));
	SubmitEditorRequest_detour::attach(GetAddress(Simulator::cGameModeManager, SubmitEditorRequest));
	UpdateLoop::AttachDetours();
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

