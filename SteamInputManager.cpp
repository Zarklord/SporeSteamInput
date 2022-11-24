#include "stdafx.h"
#include "SteamInputManager.h"

#define InitializeActionSet(name) mActionSets[InputSet::name] = mSteamInput->GetActionSetHandle(InputSetNames::name)
#define InitializeCreatureActionLayer(name) mCreatureActionLayers[InputCreatureLayer::name] = mSteamInput->GetActionSetHandle(InputLayerNames::name)
#define InitializeEditorActionLayer(name) mEditorActionLayers[InputEditorLayer::name] = mSteamInput->GetActionSetHandle(InputLayerNames::name)
#define InitializeDigitalAction(name) \
	mDigitalActions[InputDigitalAction::name] = mSteamInput->GetDigitalActionHandle(InputDigitalActionNames::name); \
	mDigitalActionLookup[mDigitalActions[InputDigitalAction::name]] = InputDigitalAction::name

SteamInputManager& GetSteamInputManager()
{
	return SteamInputManager::Get();
}

SteamInputManager* SteamInputManager::mInstance = nullptr;
void SteamInputManager::Initialize()
{
	mInstance = new SteamInputManager();
}

void SteamInputManager::Finalize()
{
	delete mInstance;
	mInstance = nullptr;
}

SteamInputManager& SteamInputManager::Get()
{
	return *mInstance;
}

void SteamInputManager::SteamInputActionEventCallback(SteamInputActionEvent_t * inputEvent)
{
	if (inputEvent->eEventType == ESteamInputActionEventType_DigitalAction)
	{
		mInstance->OnDigitalAction(inputEvent->digitalAction);
	}
	else if (inputEvent->eEventType == ESteamInputActionEventType_AnalogAction)
	{
		mInstance->OnAnalogAction(inputEvent->analogAction);
	}
}

SteamInputManager::SteamInputManager()
	: mActionSets()
	, mCreatureActionLayers()
	, mEditorActionLayers()
	, mDigitalActions()
	, mCurrentActionSet(InputSet::UI)
	, mCurrentCreatureActionLayer(InputCreatureLayer::NONE)
	, mCurrentEditorActionLayer(InputEditorLayer::NONE)
	, mSteamInput(SteamInput())
{

	mSteamInput->Init(true);
	mSteamInput->EnableDeviceCallbacks();
	mSteamInput->EnableActionEventCallbacks(SteamInputActionEventCallback);

	InitializeActionSet(UI);
	InitializeActionSet(EDITOR);
	InitializeActionSet(CELL);
	InitializeActionSet(CREATURE);
	InitializeActionSet(TRIBAL);
	InitializeActionSet(CIVILIZATION);
	InitializeActionSet(SPACE);

	InitializeCreatureActionLayer(CREATURE_CREATURE);
	InitializeCreatureActionLayer(CREATURE_ADVENTURE);

	InitializeEditorActionLayer(EDITOR_CREATURE);
	InitializeEditorActionLayer(EDITOR_BUILDING);
	InitializeEditorActionLayer(EDITOR_VEHICLE);
	InitializeEditorActionLayer(EDITOR_SPACESHIP);
	InitializeEditorActionLayer(EDITOR_ADVENTURE);

	InitializeDigitalAction(EDITOR_UNDO);
	InitializeDigitalAction(EDITOR_REDO);

	SetActionSet(InputSet::UI);
}

SteamInputManager::~SteamInputManager()
{
	mSteamInput->Shutdown();
}

void SteamInputManager::Update() const
{
	mSteamInput->RunFrame();
}

InputSet::InputSet SteamInputManager::GetActionSet() const
{
	return mCurrentActionSet;
}

void SteamInputManager::SetActionSet(InputSet::InputSet set)
{
	mCurrentActionSet = set;
	mSteamInput->ActivateActionSet(STEAM_INPUT_HANDLE_ALL_CONTROLLERS, mActionSets[mCurrentActionSet]);

	SetCreatureActionLayer(mCurrentCreatureActionLayer);
	SetEditorActionLayer(mCurrentEditorActionLayer);
}

InputCreatureLayer::InputCreatureLayer SteamInputManager::GetCreatureActionLayer() const
{
	return mCurrentCreatureActionLayer;
}

void SteamInputManager::SetCreatureActionLayer(InputCreatureLayer::InputCreatureLayer set)
{
	if (mCurrentActionSet != InputSet::CREATURE)
	{
		mCurrentCreatureActionLayer = InputCreatureLayer::NONE;
	}
	else
	{
		if (mCurrentCreatureActionLayer != InputCreatureLayer::NONE)
			mSteamInput->DeactivateActionSetLayer(STEAM_INPUT_HANDLE_ALL_CONTROLLERS, mCreatureActionLayers[mCurrentCreatureActionLayer]);

		mCurrentCreatureActionLayer = set;

		if (mCurrentCreatureActionLayer != InputCreatureLayer::NONE)
			mSteamInput->ActivateActionSetLayer(STEAM_INPUT_HANDLE_ALL_CONTROLLERS, mCreatureActionLayers[mCurrentCreatureActionLayer]);
	}
}

InputEditorLayer::InputEditorLayer SteamInputManager::GetEditorActionLayer() const
{
	return mCurrentEditorActionLayer;
}

void SteamInputManager::SetEditorActionLayer(InputEditorLayer::InputEditorLayer set)
{
	if (mCurrentActionSet != InputSet::CREATURE)
	{
		mCurrentEditorActionLayer = InputEditorLayer::NONE;
	}
	else
	{
		if (mCurrentEditorActionLayer != InputEditorLayer::NONE)
			mSteamInput->DeactivateActionSetLayer(STEAM_INPUT_HANDLE_ALL_CONTROLLERS, mEditorActionLayers[mCurrentEditorActionLayer]);

		mCurrentEditorActionLayer = set;

		if (mCurrentEditorActionLayer != InputEditorLayer::NONE)
			mSteamInput->ActivateActionSetLayer(STEAM_INPUT_HANDLE_ALL_CONTROLLERS, mEditorActionLayers[mCurrentEditorActionLayer]);
	}
}

bool SteamInputManager::IsActionHeld(InputDigitalAction::InputDigitalAction action) const
{
	for (const auto controller : mConnectedControllers)
	{
		const InputDigitalActionData_t data = mSteamInput->GetDigitalActionData(controller, mDigitalActions[action]);
		if (data.bState)
			return data.bState;
	}
	return false;
}

void SteamInputManager::OnPressedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputActionCallback callback)
{
	mOnPressedInputActionCallbacks[action] = callback;	
}

void SteamInputManager::OnReleasedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputActionCallback callback)
{
	mOnReleasedInputActionCallbacks[action] = callback;	
}

void SteamInputManager::OnSteamInputDeviceConnected(SteamInputDeviceConnected_t* pParam)
{
	mConnectedControllers.insert(pParam->m_ulConnectedDeviceHandle);
}

void SteamInputManager::OnSteamInputDeviceDisconnected(SteamInputDeviceDisconnected_t* pParam)
{
	mConnectedControllers.erase(pParam->m_ulDisconnectedDeviceHandle);
}

void SteamInputManager::OnDigitalAction(SteamInputActionEvent_t::DigitalAction_t& action)
{
	if (!action.digitalActionData.bActive) return;

	const auto it = mDigitalActionLookup.find(action.actionHandle);
	if (it != mDigitalActionLookup.end())
	{
		const InputDigitalAction::InputDigitalAction input_action = it->second;
		if (action.digitalActionData.bState)
		{
			const auto onPressed = mOnPressedInputActionCallbacks.find(input_action);
			if (onPressed != mOnPressedInputActionCallbacks.end())
				onPressed->second();
		}
		else
		{
			const auto onReleased = mOnReleasedInputActionCallbacks.find(input_action);
			if (onReleased != mOnReleasedInputActionCallbacks.end())
				onReleased->second();
		}
	}
}

void SteamInputManager::OnAnalogAction(SteamInputActionEvent_t::AnalogAction_t& action)
{
	
}
