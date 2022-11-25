#pragma once
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <steam/isteaminput.h>

namespace InputSetNames
{
	constexpr const char * UI = "UI";
	constexpr const char * EDITOR = "Editor";
	constexpr const char * ADVENTURE_EDITOR = "Adventure_Editor";
	constexpr const char * CELL = "Cell";
	constexpr const char * CREATURE = "Creature";
	constexpr const char * TRIBAL = "Tribal";
	constexpr const char * CIVILIZATION = "Civilization";
	constexpr const char * SPACE = "Space";
}

namespace InputSet
{
	enum InputSet
	{
		UI,
		EDITOR,
		ADVENTURE_EDITOR,
		CELL,
		CREATURE,
		TRIBAL,
		CIVILIZATION,
		SPACE,
		COUNT,
	};
}

namespace InputLayerNames
{
	constexpr const char * CREATURE_CREATURE = "Creature_Creature";
	constexpr const char * CREATURE_ADVENTURE = "Creature_Adventure";
	constexpr const char * EDITOR_CREATURE = "Editor_Creature";
	constexpr const char * EDITOR_BUILDING = "Editor_Building";
	constexpr const char * EDITOR_VEHICLE = "Editor_Vehicle";
	constexpr const char * EDITOR_SPACESHIP = "Editor_Spaceship";
}

namespace InputCreatureLayer
{
	enum InputCreatureLayer
	{
		CREATURE_CREATURE,
		CREATURE_ADVENTURE,
		COUNT,
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
		COUNT,
		NONE,
	};
}

namespace InputDigitalActionNames
{
	constexpr const char * EDITOR_UNDO = "editor_undo";
	constexpr const char * EDITOR_REDO = "editor_redo";
}

namespace InputDigitalAction
{
	enum InputDigitalAction
	{
		EDITOR_UNDO,
		EDITOR_REDO,
		COUNT,
	};
}

namespace InputAnalogActionNames
{
}

namespace InputAnalogAction
{
	enum InputAnalogAction
	{
		COUNT,
	};
}

typedef void (*SteamInputDigitalActionCallback)();
typedef void (*SteamInputAnalogActionCallback)(EInputSourceMode mode, float x, float y);

class SteamInputManager : public DefaultRefCounted, public App::IUpdatable
{
public:
	static void Initialize();
	static void Finalize();
	static SteamInputManager& Get();
	static void SteamInputActionEventCallback(SteamInputActionEvent_t * inputEvent);
private:
	SteamInputManager();
public:
	virtual ~SteamInputManager();

	void OnPostInit();

	void Update();

	void SetActionSet(InputSet::InputSet set);
	InputSet::InputSet GetActionSet() const;

	void SetCreatureActionLayer(InputCreatureLayer::InputCreatureLayer set);
	InputCreatureLayer::InputCreatureLayer GetCreatureActionLayer() const;

	void SetEditorActionLayer(InputEditorLayer::InputEditorLayer set);
	InputEditorLayer::InputEditorLayer GetEditorActionLayer() const;

	bool IsActionHeld(InputDigitalAction::InputDigitalAction action) const;

	void OnPressedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputDigitalActionCallback callback);
	void OnReleasedInputAction(InputDigitalAction::InputDigitalAction action, SteamInputDigitalActionCallback callback);
	void OnAnalogInputAction(InputAnalogAction::InputAnalogAction action, SteamInputAnalogActionCallback callback);

private:
	static SteamInputManager* mInstance;

	STEAM_CALLBACK(SteamInputManager, OnSteamInputDeviceConnected, SteamInputDeviceConnected_t);
	STEAM_CALLBACK(SteamInputManager, OnSteamInputDeviceDisconnected, SteamInputDeviceDisconnected_t);
	
	void OnDigitalAction(SteamInputActionEvent_t::DigitalAction_t & action);
	void OnAnalogAction(SteamInputActionEvent_t::AnalogAction_t & action);

	virtual int AddRef() override;
	virtual int Release() override;

	std::array<InputActionSetHandle_t, InputSet::COUNT> mActionSets;
	std::array<InputActionSetHandle_t, InputCreatureLayer::COUNT> mCreatureActionLayers;
	std::array<InputActionSetHandle_t, InputEditorLayer::COUNT> mEditorActionLayers;

	std::array<InputDigitalActionHandle_t, InputDigitalAction::COUNT> mDigitalActions;
	std::unordered_map<InputDigitalActionHandle_t, InputDigitalAction::InputDigitalAction> mDigitalActionLookup;
	std::unordered_map<InputDigitalAction::InputDigitalAction, SteamInputDigitalActionCallback> mOnPressedInputActionCallbacks;
	std::unordered_map<InputDigitalAction::InputDigitalAction, SteamInputDigitalActionCallback> mOnReleasedInputActionCallbacks;

	std::array<InputAnalogActionHandle_t, InputAnalogAction::COUNT> mAnalogActions;
	std::unordered_map<InputAnalogActionHandle_t, InputAnalogAction::InputAnalogAction> mAnalogActionsLookup;
	std::unordered_map<InputAnalogAction::InputAnalogAction, SteamInputAnalogActionCallback> mOnAnalogActionCallbacks;

	std::unordered_set<InputHandle_t> mConnectedControllers;

	InputSet::InputSet mCurrentActionSet;
	InputCreatureLayer::InputCreatureLayer mCurrentCreatureActionLayer;
	InputEditorLayer::InputEditorLayer mCurrentEditorActionLayer;

	intrusive_ptr<App::UpdateMessageListener> mUpdateMessageListener;
	ISteamInput * mSteamInput;
};

SteamInputManager& GetSteamInputManager();

