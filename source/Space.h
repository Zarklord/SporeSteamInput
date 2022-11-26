#pragma once
#include "SteamInputManager.h"

namespace SpaceSteamControls
{

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
	}

	inline void AttachDetours()
	{
	}
}
