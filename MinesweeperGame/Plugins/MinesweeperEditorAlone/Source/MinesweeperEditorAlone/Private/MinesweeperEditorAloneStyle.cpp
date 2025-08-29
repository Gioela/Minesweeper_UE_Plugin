// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditorAloneStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

//#define RootToContentDir Style->RootToContentDir

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
const FVector2D Icon260x280(260.0f, 280.0f);

TSharedRef< FSlateStyleSet > FMinesweeperEditorAloneStyle::Create()
{
	//TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MinesweeperEditorAloneStyle"));
	//Style->SetContentRoot(IPluginManager::Get().FindPlugin("MinesweeperEditorAlone")->GetBaseDir() / TEXT("Resources"));
	//Style->Set("MinesweeperEditorAlone.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

	FString RootPath = IPluginManager::Get().FindPlugin("MinesweeperEditorAlone")->GetBaseDir() + TEXT("/Resources");
	Style->SetContentRoot(RootPath);

	//Style->Set("MinesweeperEditorAlone.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon260x280));
	Style->Set("MinesweeperEditorAlone.OpenPluginWindow", new FSlateImageBrush(Style->RootToContentDir(TEXT("mina.png")), FVector2D(40.0f, 40.0f)));




	FString IconPath = RootPath + TEXT("/mina.png");
	FSlateImageBrush* IconBrush48 = new FSlateImageBrush(IconPath, FVector2D(48.f, 48.f));
	FSlateImageBrush* IconBrush32 = new FSlateImageBrush(IconPath, FVector2D(32.f, 32.f));
	FSlateImageBrush* IconBrush16 = new FSlateImageBrush(IconPath, FVector2D(16.f, 16.f));

	FString GameIconPath = RootPath + TEXT("/mina.png");
	FSlateImageBrush* GameIconBrush = new FSlateImageBrush(GameIconPath, FVector2D(16.f, 16.f));
	FSlateImageBrush* MineIconBrush = new FSlateImageBrush(GameIconPath, FVector2D(8.f, 8.f));


	Style->Set("Minesweeper.GameIcon", GameIconBrush);
	Style->Set("Minesweeper.MineIcon", MineIconBrush);

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
