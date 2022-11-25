#pragma once

class SteamInputManager;

class GameModeListener : public App::DefaultMessageListener
{
public:
	GameModeListener(SteamInputManager& steam_input_manager);

	bool HandleMessage(uint32_t messageID, void* message) override;
private:
	SteamInputManager& mSteamInputManager;
};

