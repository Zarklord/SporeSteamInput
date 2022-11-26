#pragma once
#include "SteamInputManager.h"

namespace AdventureSteamControls
{

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
	}

	inline void AttachDetours()
	{
	}
}
