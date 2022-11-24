#include "stdafx.h"
#include "SteamInputCheat.h"

#include "SteamInputManager.h"

SteamInputCheat::SteamInputCheat(SteamInputManager& steam_input_manager)
	: mSteamInputManager(steam_input_manager)
{
}

void SteamInputCheat::ParseLine(const ArgScript::Line& line)
{
	mSteamInputManager.SetActionSet(InputSet::UI);

}

const char* SteamInputCheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	return "Sets the steam input action set to UI";
}
