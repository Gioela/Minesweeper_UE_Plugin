// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditorAloneCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperEditorAloneModule"

void FMinesweeperEditorAloneCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MinesweeperEditorAlone", "Open MinesweeperEditorAlone window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
