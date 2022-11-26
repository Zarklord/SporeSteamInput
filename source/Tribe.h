#pragma once
#include "SteamInputManager.h"

namespace TribeSteamControls
{

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
	}

	inline void AttachDetours()
	{
	}
}
