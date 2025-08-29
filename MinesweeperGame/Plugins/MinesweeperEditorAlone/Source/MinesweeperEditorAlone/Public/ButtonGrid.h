// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
//#include "Widgets/Input/SButton.h"
//#include "Widgets/Text/STextBlock.h"

DECLARE_DELEGATE(FOnMineClicked);
DECLARE_DELEGATE_OneParam(FOnCellClicked, int32);

/**
 * A custom Slate widget that creates a customizable grid of buttons.
 * The number of rows and columns can be specified via Slate arguments.
 * Clicking a button opens a message box with its row and column index.
 */
//class SButtonGrid : public SButton
class MINESWEEPEREDITORALONE_API SButtonGrid : public SCompoundWidget
{
public:
    // Slate arguments allow customization when creating the widget.
    SLATE_BEGIN_ARGS(SButtonGrid)
        : _bIsMine(false)
        , _bVisible(true)
        , _bIsClicked(false)
        , _MineNeighbor(0)
        {}
        SLATE_ARGUMENT(bool, bIsMine)
        SLATE_ARGUMENT(bool, bVisible)
        SLATE_ARGUMENT(bool, bIsClicked)
        SLATE_ARGUMENT(int32, MineNeighbor)
        
        SLATE_ATTRIBUTE(FText, Text)
        SLATE_EVENT(FOnClicked, OnClicked)
        SLATE_EVENT(FOnMineClicked, OnMineClicked)
        SLATE_EVENT(FOnCellClicked, OnCellClicked)
    SLATE_END_ARGS()

public:
    /**
     * Constructs this widget.
     * This is where the widget hierarchy is built.
     * @param InArgs The Slate arguments passed during creation.
     */
    void Construct(const FArguments& InArgs);
    void Initialize();

    bool GetIsMine() const;
    void SetIsMine(bool InValue);

    bool GetIsClicked() const;
    void SetClicked(bool InValue);

    int32 GetMineNeighbor();
    void AddMinaNeighbor();

    void ResetButton();
    void SetGameOver();
    FSlateColor SetButtonTransparency() const;

    int32 CellIndex;

private:

    int32 MineNeighbor;
    bool bIsMine;
    bool bVisible;
    bool bIsClicked;
    bool bGameOver = false;

    FOnCellClicked OnCellClicked;
    FOnMineClicked OnMineClicked;

private:

    void OnVisulaUpdate();
    FReply OnButtonClicked();
    FText GetTextOnButton() const;
};
