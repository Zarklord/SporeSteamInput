#pragma once
#include "SteamInputManager.h"

namespace CreatureSteamControls
{

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
	}

	inline void AttachDetours()
	{
	}
}
