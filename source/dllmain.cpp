// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <steam/steam_api.h>
#include <Spore/App/cGameModeManager.h>

#include "Editor.h"
#include "AdventureEditor.h"
#include "MainMenu.h"
#include "Cell.h"
#include "Creature.h"
#include "Tribe.h"
#include "Civilization.h"
#include "Space.h"
#include "Adventure.h"

#include "GameModeListener.h"
#include "SteamInputManager.h"

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

intrusive_ptr<App::UpdateMessageListener> sSteamAPIRunCallbacksUpdater;

void PostInitialize()
{
	//ManualBreakpoint();

	SteamInputManager& steam_input_manager = GetSteamInputManager();

	steam_input_manager.OnPostInit();
	sSteamAPIRunCallbacksUpdater = App::AddUpdateFunction(SteamAPI_RunCallbacks);

	MessageManager.AddListener(new GameModeListener(steam_input_manager), App::OnModeEnterMessage::ID);

	MainMenuSteamControls::Initialize();
	EditorSteamControls::Initialize();
	AdventureEditorSteamControls::Initialize();
	CellSteamControls::Initialize();
	CreatureSteamControls::Initialize();
	TribeSteamControls::Initialize();
	CivilizationSteamControls::Initialize();
	SpaceSteamControls::Initialize();
	AdventureSteamControls::Initialize();
}

void Dispose()
{
	App::RemoveUpdateFunction(sSteamAPIRunCallbacksUpdater);
	sSteamAPIRunCallbacksUpdater = nullptr;
	// This method is called when the game is closing
}

void AttachDetours()
{
	MainMenuSteamControls::AttachDetours();
	EditorSteamControls::AttachDetours();
	AdventureEditorSteamControls::AttachDetours();
	CellSteamControls::AttachDetours();
	CreatureSteamControls::AttachDetours();
	TribeSteamControls::AttachDetours();
	CivilizationSteamControls::AttachDetours();
	SpaceSteamControls::AttachDetours();
	AdventureSteamControls::AttachDetours();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			gSteamAPI_Init = RealMain();

			if (!gSteamAPI_Init) break;
			
			ModAPI::AddPostInitFunction(PostInitialize);
			ModAPI::AddDisposeFunction(Dispose);

			PrepareDetours(hModule);
			AttachDetours();
			CommitDetours();
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			if (gSteamAPI_Init)
				RealExit();
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default:
		{
			break;
		}
	}
	return TRUE;
}

