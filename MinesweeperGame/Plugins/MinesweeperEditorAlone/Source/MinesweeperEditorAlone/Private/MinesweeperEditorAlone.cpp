// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditorAlone.h"
#include "MinesweeperEditorAloneStyle.h"
#include "MinesweeperEditorAloneCommands.h"
#include "LevelEditor.h"
#include "Widgets/Colors/SSimpleGradient.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MinesweeperEditorAloneTabName("MinesweeperEditorAlone");

#define LOCTEXT_NAMESPACE "FMinesweeperEditorAloneModule"

DEFINE_LOG_CATEGORY(MinesweeperLog);

void FMinesweeperEditorAloneModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperEditorAloneStyle::Initialize();
	FMinesweeperEditorAloneStyle::ReloadTextures();

	FMinesweeperEditorAloneCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperEditorAloneCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperEditorAloneModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperEditorAloneModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperEditorAloneTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperEditorAloneModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMinesweeperEditorAloneTabTitle", "MinesweeperEditorAlone"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);

	// Initialize the Mine Grid
	Level = new MinesweeperLevel(3, 3, 2);
}

void FMinesweeperEditorAloneModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperEditorAloneStyle::Shutdown();

	FMinesweeperEditorAloneCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperEditorAloneTabName);
}

TSharedRef<SDockTab> FMinesweeperEditorAloneModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.ContentPadding(FMargin(5.f))
		[
			SNew(SBox)
				.Padding(5.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.Padding(5.f, 0.f)
					.MinWidth(150.f)
					[
						FMinesweeperEditorAloneModule::GenerateSettingsBox()
					]
				+ SHorizontalBox::Slot()
					.Padding(5.f, 0.f)
					.MinWidth(150.f)
					[
						FMinesweeperEditorAloneModule::GenerateGameBox()
					]

			]
		];
}

void FMinesweeperEditorAloneModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperEditorAloneTabName);
}

// check the rows input value and update it WITHOUT update the grid
// to update the game grid it is necessary click on the button "Update"
void FMinesweeperEditorAloneModule::OnChangeRows(const FText& InValue, ETextCommit::Type CommitType)
{
	int32 Rows = FCString::Atoi(*RowsBox->GetText().ToString());

	if (!Level || Rows < 1 || Rows > 15)
	{
		UE_LOG(MinesweeperLog, Warning, TEXT("Rows value is not valid and cannot be changed"));

		FText DialogText = LOCTEXT("ErrorInputValue", "Rows value is not valid and cannot be changed");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);

		RowsBox->SetText(FText::AsNumber(Level->GetRows()));

		return;
	}

	UE_LOG(MinesweeperLog, Warning, TEXT("Rows was changed. Input: %d to %d"), Level->GetMines(), Rows);

	Level->SetRows(Rows);
}

// check the columns input value and update it WITHOUT update the grid
// to update the game grid it is necessary click on the button "Update"
void FMinesweeperEditorAloneModule::OnChangeColumns(const FText& InValue, ETextCommit::Type CommitType)
{
	int32 Columns = FCString::Atoi(*ColumnsBox->GetText().ToString());

	if (!Level || Columns < 1 || Columns > 15)
	{
		UE_LOG(MinesweeperLog, Warning, TEXT("Columns value is not valid and cannot be changed"));

		FText DialogText = LOCTEXT("ErrorInputValue", "Columns value is not valid and cannot be changed");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);

		ColumnsBox->SetText(FText::AsNumber(Level->GetColumns()));

		return;
	}

	UE_LOG(MinesweeperLog, Warning, TEXT("Columns was changed. Input: %d to %d"), Level->GetColumns(), Columns);

	Level->SetMines(Columns);
}

// check the mine input value and update it WITHOUT update the grid
// to update the game grid it is necessary click on the button "Update"
void FMinesweeperEditorAloneModule::OnChangeMines(const FText& InValue, ETextCommit::Type CommitType)
{
	int32 Mines = FCString::Atoi(*MinesBox->GetText().ToString());

	// TODO ADD Level to get max mines value as percentage of level size
	if (!Level || Mines < 1 || Mines > Level->GetSize() / 2 )
	{
		UE_LOG(MinesweeperLog, Warning, TEXT("Mines value is not valid and cannot be changed"));

		FText DialogText = LOCTEXT("ErrorInputValue", "Mines value is not valid and cannot be changed");
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);

		// restore last valid mine number
		MinesBox->SetText(FText::AsNumber(Level->GetMines()));

		return;
	}

	UE_LOG(MinesweeperLog, Warning, TEXT("Mines was changed. Input: %d to %d"), Level->GetMines(), Mines);

	Level->SetMines(Mines);
}

// Update the grid level with all properties in the window
void FMinesweeperEditorAloneModule::OnUpdateGridButtonClicked()
{
	int32 Rows = FCString::Atoi(*RowsBox->GetText().ToString());
	int32 Columns = FCString::Atoi(*ColumnsBox->GetText().ToString());
	int32 Mines = FCString::Atoi(*MinesBox->GetText().ToString());

	Level->UpdateLevelDetails(Rows, Columns, Mines);
	Level->GenerateGrid();

	UE_LOG(MinesweeperLog, Warning, TEXT("Grid updated to: %d rows, %d columns, %d mines"), Rows, Columns, Mines);
}

// Generate the settings box on the left side of the plugin tab
// This contains text boxes to input grid settings and a button to update the grid
// Returns a shared reference to the settings box
TSharedRef<SHorizontalBox> FMinesweeperEditorAloneModule::GenerateSettingsBox()
{
	TSharedRef<SHorizontalBox> SettingsBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Green)
				.Padding(10)
				[
					SNew(SBox)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Center)
								.Padding(10)
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.AutoHeight()
										[
											SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.AutoWidth()
												.VAlign(VAlign_Center)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("GridRows", "Grid Rows: "))
														.ToolTipText(LOCTEXT("GridRowsTooltip", "This is the number of rows for the game grid"))
												]
												+ SHorizontalBox::Slot()
												.MinWidth(30.f)
												.VAlign(VAlign_Center)
												[
													// Editable text box for rows number. Default value is 3
													SAssignNew(RowsBox, SEditableTextBox)
														//.Text(FText::FromString("3"))
														.Text(FText::AsNumber(Level->GetRows()))
														.MinDesiredWidth(50.f)
														.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperEditorAloneModule::OnChangeRows))
												]
												+ SHorizontalBox::Slot()
												[
													SNew(SSeparator)
														.Visibility(EVisibility::Hidden)
												]
												+ SHorizontalBox::Slot()
												.AutoWidth()
												.VAlign(VAlign_Center)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("GridColumns", "Grid Columns: "))
														.ToolTipText(LOCTEXT("GridColumnsTooltip", "This is the number of columns for the game grid"))
												]
												+ SHorizontalBox::Slot()
												.MinWidth(30.f)
												.VAlign(VAlign_Center)
												[
													// Editable text box for columns number. Default value is 3
													SAssignNew(ColumnsBox, SEditableTextBox)
														//.Text(FText::FromString("3"))
														.Text(FText::AsNumber(Level->GetColumns()))
														.MinDesiredWidth(50.f)
														.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperEditorAloneModule::OnChangeColumns))
												]
										]
									+ SVerticalBox::Slot()
										.AutoHeight()
										[
											SNew(SSeparator)
												.Visibility(EVisibility::Hidden)
										]
										+ SVerticalBox::Slot()
										.AutoHeight()
										[
											SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.AutoWidth()
												.VAlign(VAlign_Center)
												[
													SNew(STextBlock)
														.Text(LOCTEXT("MinesInGrid", "Total Mines In Grid: "))
														.ToolTipText(LOCTEXT("MinesInGridTooltip", "Total mines in the game grid"))
												]
												+ SHorizontalBox::Slot()
												.MinWidth(30.f)
												.VAlign(VAlign_Center)
												[
													// Editable text box for mines number. Default value is 2
													SAssignNew(MinesBox, SEditableTextBox)
														//.Text(FText::FromString("2"))
														.Text(FText::AsNumber(Level->GetMines()))
														.MinDesiredWidth(50.f)
														.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &FMinesweeperEditorAloneModule::OnChangeMines))
												]
										]
								]
							+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SButton)
										.Text(LOCTEXT("UpdateGameGridProperties", "Update Game Grid"))
										.OnClicked_Lambda([this]()->FReply
											{
												FMinesweeperEditorAloneModule::OnUpdateGridButtonClicked();
												return FReply::Handled();
											}
										)
								]
							// Log for DEBUG Level values
							+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SButton)
										.Text(LOCTEXT("ButtonText", "Print Log Level Info"))
										.OnClicked_Lambda(
											[this]() -> FReply
											{
												FMinesweeperEditorAloneModule::PrintLevelProperties();
												return FReply::Handled();
											}
										)
								]
						]
				]
		];
	return SettingsBox;
}

TSharedRef<SHorizontalBox> FMinesweeperEditorAloneModule::GenerateGameBox()
{
	TSharedRef<SHorizontalBox> GameBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Red)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.MinHeight(20.f)
						.AutoHeight()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("GameStatus", "Status Game"))
						]
						+ SVerticalBox::Slot()
						.FillHeight(1.0f)
						.Padding(10)
						[
							SNew(SBorder)
								.BorderBackgroundColor(FLinearColor::Yellow)
								.VAlign(EVerticalAlignment::VAlign_Center)
								.HAlign(EHorizontalAlignment::HAlign_Center)
								.Padding(2.f)
								[
									SAssignNew(Level->MinesweeperGridWidget, SMinesweeperGrid)
										//SAssignNew(MinesweeperGridWidget, SMinesweeperGrid)
										.Rows(Level->GetRows())
										.Columns(Level->GetColumns())
										//.Mines(Level->GetMines())
								]
						]
				]
		];
	return GameBox;
}

void FMinesweeperEditorAloneModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperEditorAloneCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperEditorAloneCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

// for debugging
void FMinesweeperEditorAloneModule::PrintLevelProperties()
{
	if (!Level)
	{
		UE_LOG(MinesweeperLog, Warning, TEXT("No Level exists"));
	}
	else
	{
		UE_LOG(MinesweeperLog, Warning,
			TEXT("Level properties\tRows: %d | Cols: %d | Mines: %d | LevelSize: %d"),
			Level->GetRows(), Level->GetColumns(), Level->GetMines(), Level->GetSize()
		);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperEditorAloneModule, MinesweeperEditorAlone)