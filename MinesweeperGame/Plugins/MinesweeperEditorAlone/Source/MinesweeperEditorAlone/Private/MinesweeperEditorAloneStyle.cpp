// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditorAloneStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperEditorAloneStyle::StyleInstance = nullptr;

void FMinesweeperEditorAloneStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMinesweeperEditorAloneStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMinesweeperEditorAloneStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperStyleA"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMinesweeperEditorAloneStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MinesweeperEditorAlone")->GetBaseDir() / TEXT("Resources"));
	Style->Set("MinesweeperEditorAlone.OpenPluginWindow", new IMAGE_BRUSH(TEXT("MinaCopilot"), Icon20x20));

	return Style;
}

void FMinesweeperEditorAloneStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMinesweeperEditorAloneStyle::Get()
{
	return *StyleInstance;
}
