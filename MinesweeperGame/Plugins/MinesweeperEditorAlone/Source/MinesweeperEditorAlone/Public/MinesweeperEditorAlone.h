// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include <MinesweeperLevel.h>

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperEditorAloneModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	void OnChangeRows(const FText& InValue, ETextCommit::Type CommitType);
	void OnChangeColumns(const FText& InValue, ETextCommit::Type CommitType);
	void OnChangeMines(const FText& InValue, ETextCommit::Type CommitType);

	void OnUpdateGridButtonClicked();

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<SEditableTextBox> RowsBox;
	TSharedPtr<SEditableTextBox> ColumnsBox;
	TSharedPtr<SEditableTextBox> MinesBox;

	MinesweeperLevel* Level;

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedRef<SHorizontalBox> GenerateSettingsBox();
	TSharedRef<SHorizontalBox> GenerateGameBox();
	void PrintLevelProperties();
};

DECLARE_LOG_CATEGORY_EXTERN(MinesweeperLog, Log, All)
