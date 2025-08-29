// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "MinesweeperEditorAloneStyle.h"

class FMinesweeperEditorAloneCommands : public TCommands<FMinesweeperEditorAloneCommands>
{
public:

	FMinesweeperEditorAloneCommands()
		: TCommands<FMinesweeperEditorAloneCommands>(TEXT("MinesweeperEditorAlone")
		, NSLOCTEXT("Contexts", "MinesweeperEditorAlone", "MinesweeperEditorAlone Plugin")
		, NAME_None
		, FMinesweeperEditorAloneStyle::GetStyleSetName()
		)
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};