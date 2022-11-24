#pragma once
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <steam/isteaminput.h>

namespace InputSetNames
{
	constexpr const char * UI = "UI";
	constexpr const char * EDITOR = "Editor";
	constexpr const char * CELL = "Cell";
	constexpr const char * CREATURE = "Creature";
	constexpr const char * TRIBAL = "Tribal";
	constexpr const char * CIVILIZATION = "Civilization";
	constexpr const char * SPACE = "Space";
}


namespace InputLayerNames
{
	constexpr const char * CREATURE_CREATURE = "Creature_Creature";
	constexpr const char * CREATURE_ADVENTURE = "Creature_Adventure";
	constexpr const char * EDITOR_CREATURE = "Editor_Creature";
	constexpr const char * EDITOR_BUILDING = "Editor_Building";
	constexpr const char * EDITOR_VEHICLE = "Editor_Vehicle";
	constexpr const char * EDITOR_SPACESHIP = "Editor_Spaceship";
	constexpr const char * EDITOR_ADVENTURE = "Editor_Adventure";
}

namespace InputDigitalActionNames
{
	constexpr const char * EDITOR_UNDO = "editor_undo";
	constexpr const char * EDITOR_REDO = "editor_redo";
}

namespace InputSet
{
	enum InputSet
	{
		UI,
		EDITOR,
		CELL,
		CREATURE,
		TRIBAL,
		CIVILIZATION,
		SPACE,
		SIZE,
	};
}

namespace InputCreatureLayer
{
	enum InputCreatureLayer
	{
		CREATURE_CREATURE,
		CREATURE_ADVENTURE,
		SIZE,
		NONE,
	};
}

namespace InputEditorLayer
{
	enum InputEditorLayer
	{
		EDITOR_CREATURE,
		EDITOR_BUILDING,
		EDITOR_VEHICLE,
		EDITOR_SPACESHIP,
		EDITOR_ADVENTURE,
		SIZE,
		NONE,
	};
}

namespace InputDigitalAction
{
	enum InputDigitalAction
	{
		EDITOR_UNDO,
		EDITOR_REDO,
		SIZE,
	};
}

typedef void (*SteamInputActionCallback)();

class SteamInputManager
{
public:
	static void Initialize();
	static void Finalize();
	static SteamInputManager& Get();
	static void SteamInputActionEventCallback(SteamInputActionEvent_t * inputEvent);
private:
	SteamInputManager();
public:
	~SteamInputManager();

	void Update() const;

	void SetActionSet(InputSet::InputSet set);
	InputSet::InputSet GetActionSet() const;

	void SetCreatureActionLayer(InputCreatureLayer::InputCreatureLayer set);
	InputCreatureLayer::InputCreatureLayer GetCreatureActionLayer() const;

	void SetEditorActionLayer(InputEditorLayer::InputEditorLayer set);
	InputEditorLayer::InputEditorLayer GetEditorActionLayer() const;

	bool IsActionHeld(InputDigitalAction::InputDigitalAction action) const;

	void OnPressedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputActionCallback callback);
	void OnReleasedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputActionCallback callback);

private:
	static SteamInputManager* mInstance;

	STEAM_CALLBACK(SteamInputManager, OnSteamInputDeviceConnected, SteamInputDeviceConnected_t);
	STEAM_CALLBACK(SteamInputManager, OnSteamInputDeviceDisconnected, SteamInputDeviceDisconnected_t);
	
	void OnDigitalAction(SteamInputActionEvent_t::DigitalAction_t & action);
	void OnAnalogAction(SteamInputActionEvent_t::AnalogAction_t & action);

	std::array<InputActionSetHandle_t, InputSet::SIZE> mActionSets;
	std::array<InputActionSetHandle_t, InputCreatureLayer::SIZE> mCreatureActionLayers;
	std::array<InputActionSetHandle_t, InputEditorLayer::SIZE> mEditorActionLayers;

	std::array<InputDigitalActionHandle_t, InputDigitalAction::SIZE> mDigitalActions;
	std::unordered_map<InputDigitalActionHandle_t, InputDigitalAction::InputDigitalAction> mDigitalActionLookup;
	
	std::unordered_map<InputHandle_t, SteamInputActionCallback> mOnPressedInputActionCallbacks;
	std::unordered_map<InputHandle_t, SteamInputActionCallback> mOnReleasedInputActionCallbacks;

	std::unordered_set<InputHandle_t> mConnectedControllers;

	InputSet::InputSet mCurrentActionSet;
	InputCreatureLayer::InputCreatureLayer mCurrentCreatureActionLayer;
	InputEditorLayer::InputEditorLayer mCurrentEditorActionLayer;

	ISteamInput * mSteamInput;
};

SteamInputManager& GetSteamInputManager();

