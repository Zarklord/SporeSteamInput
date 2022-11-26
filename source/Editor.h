#pragma once
#include "SteamInputManager.h"

namespace EditorSteamControls
{

	//CTRL + N is "new creation"
	inline void OnEditorNewPressed()
	{
		KeyModifiers modifiers{0};
		modifiers.IsCtrlDown = 1;
		Editor.OnKeyDown('N', modifiers);
	}

	inline void OnEditorNewReleased()
	{
		Editor.OnKeyUp('N', {0});
	}

	inline void OnEditorUndoPressed()
	{
		Editor.Undo(true, true);
	}

	inline void OnEditorRedoPressed()
	{
		Editor.Redo();
	}

	inline void Initialize()
	{
		SteamInputManager& steam_input_manager = GetSteamInputManager();
		
		steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_NEW, OnEditorNewPressed);
		steam_input_manager.OnReleasedInputAction(InputDigitalAction::EDITOR_NEW, OnEditorNewReleased);

		steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_UNDO, OnEditorUndoPressed);
		steam_input_manager.OnPressedInputAction(InputDigitalAction::EDITOR_REDO, OnEditorRedoPressed);
	}

	inline void AttachDetours()
	{
	}
}
