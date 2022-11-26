#pragma once
#include "SteamInputManager.h"

namespace AdventureEditorSteamControls
{

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
	}

	inline void AttachDetours()
	{
	}
}
