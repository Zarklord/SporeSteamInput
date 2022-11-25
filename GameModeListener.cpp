#include "stdafx.h"

#include <Spore/App/ScenarioMode.h>

#include "GameModeListener.h"
#include "SteamInputManager.h"

GameModeListener::GameModeListener(SteamInputManager& steam_input_manager)
	: mSteamInputManager(steam_input_manager)
{
}

bool GameModeListener::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == App::OnModeEnterMessage::ID) 
    {
        const auto data = static_cast<App::OnModeEnterMessage*>(message);

        switch (data->GetModeID())
        {
			case GameModeIDs::kGameCell:
	        {
				mSteamInputManager.SetActionSet(InputSet::CELL);
		        break;
	        }
			case GameModeIDs::kGameCreature:
	        {
				mSteamInputManager.SetActionSet(InputSet::CREATURE);
				mSteamInputManager.SetCreatureActionLayer(InputCreatureLayer::CREATURE_CREATURE);
		        break;
	        }
			case GameModeIDs::kGameTribe:
	        {
				mSteamInputManager.SetActionSet(InputSet::TRIBAL);
		        break;
	        }
			case GameModeIDs::kGameCiv:
	        {
				mSteamInputManager.SetActionSet(InputSet::CIVILIZATION);
		        break;
	        }
			case GameModeIDs::kGameSpace:
	        {
				mSteamInputManager.SetActionSet(InputSet::SPACE);
		        break;
	        }
			case GameModeIDs::kScenarioMode:
	        {
				switch(ScenarioMode.mMode)
				{
					case App::cScenarioMode::Mode::EditMode:
					{
						mSteamInputManager.SetActionSet(InputSet::ADVENTURE_EDITOR);
						break;
					}
					case App::cScenarioMode::Mode::PlayMode:
					{
						mSteamInputManager.SetActionSet(InputSet::CREATURE);
						mSteamInputManager.SetCreatureActionLayer(InputCreatureLayer::CREATURE_ADVENTURE);
						break;
					}
					default: break;
				}
		        break;
	        }
			case GameModeIDs::kEditorMode:
	        {
				mSteamInputManager.SetActionSet(InputSet::EDITOR);

				switch(Editor.mSaveExtension)
				{
					case TypeIDs::Names::cll:
					case TypeIDs::Names::crt:
					{
						mSteamInputManager.SetEditorActionLayer(InputEditorLayer::EDITOR_CREATURE);
						break;
					}
					case TypeIDs::Names::bld:
					{
						mSteamInputManager.SetEditorActionLayer(InputEditorLayer::EDITOR_BUILDING);
						break;
					}
					case TypeIDs::Names::vcl:
					{
						mSteamInputManager.SetEditorActionLayer(InputEditorLayer::EDITOR_VEHICLE);
						break;
					}
					case TypeIDs::Names::ufo:
					{
						mSteamInputManager.SetEditorActionLayer(InputEditorLayer::EDITOR_SPACESHIP);
						break;
					}
					default: break;
				}
		        break;
	        }
			case GameModeIDs::kGGEMode:
	        {
				mSteamInputManager.SetActionSet(InputSet::UI);
		        break;
	        }
			default:
			{
				//unknown game mode fall back to main menu UI so they can atleast navigate out of the menus.
				mSteamInputManager.SetActionSet(InputSet::UI);
				break;	
			}
        }
    }
	return false;
}