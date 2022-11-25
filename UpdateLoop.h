#pragma once
#include <Spore/BasicIncludes.h>
#include <Spore/App/cCameraManager.h>
#include <steam/steam_api.h>

#include "SteamInputManager.h"

namespace UpdateLoop
{
	long AttachDetours();

	virtual_detour(Update__detour, App::cCameraManager, App::ICameraManager, void(int))
	{
		void detoured(int deltaTime)
		{
			original_function(this, deltaTime);
			GetSteamInputManager().Update();
		}
	};
};

