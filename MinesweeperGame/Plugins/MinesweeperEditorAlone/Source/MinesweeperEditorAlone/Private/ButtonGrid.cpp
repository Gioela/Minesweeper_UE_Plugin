// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonGrid.h"
#include "Widgets/Text/STextBlock.h"
#include "Misc/MessageDialog.h"

void SButtonGrid::Construct(const FArguments& InArgs)
{
    bIsMine = InArgs._bIsMine;
    bIsClicked = InArgs._bIsClicked;
    MineNeighbor = InArgs._MineNeighbor;

    ChildSlot
        [
            SNew(SButton)
                .OnClicked(this, &SButtonGrid::OnButtonClicked)
                .Content()
                [
                    SNew(STextBlock)
                        .Text(this, &SButtonGrid::GetTextOnButton)
                ]
                .ButtonColorAndOpacity(this, &SButtonGrid::SetButtonTransparency)
        ];

    OnCellClicked = InArgs._OnCellClicked;
}

void SButtonGrid::Initialize()
{
    ResetButton();
}

FReply SButtonGrid::OnButtonClicked()
{
    if (bGameOver || bIsClicked)
    {
        return FReply::Handled();
    }

    if (bIsMine)
    {
        bGameOver = true;
    }

    OnCellClicked.ExecuteIfBound(CellIndex);
    bIsClicked = true;

    return FReply::Handled();
}

// Automatically executed when button was pressed
FText SButtonGrid::GetTextOnButton() const
{
    // Return the appropriate text based on the button's state
    if (!bIsClicked)
    {
        return FText::FromString(TEXT(""));
    }

    if (bIsMine)
    {
        return FText::FromString(TEXT("*"));
    }

    if (MineNeighbor > 0)
    {
        return FText::AsNumber(MineNeighbor);
    }

    return FText::FromString(TEXT(""));
}

// Automatically called by the STextBox in SButtonGrid constructor
FSlateColor SButtonGrid::SetButtonTransparency() const
{
    FSlateColor Color = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.f));

    if (bIsClicked || bGameOver)
    {
        if (bIsMine)
        {
            Color = FSlateColor(FLinearColor::Red);
        }
        else
        {
            Color = FSlateColor(FLinearColor(0.0f, 1.f, 1.f, 0.3f));
        }
    }
    return Color;
}

void SButtonGrid::RevealNeighbord()
{
    bIsClicked = true;
    SetButtonTransparency();
    GetTextOnButton();
}

bool SButtonGrid::GetIsMine() const
{
    return bIsMine;
}

void SButtonGrid::SetIsMine(bool bInValue)
{
    bIsMine = bInValue;
}

bool SButtonGrid::GetIsClicked() const
{
    return bIsClicked;
}

void SButtonGrid::SetClicked(bool bInValue)
{
    bIsClicked = bInValue;
}

int32 SButtonGrid::GetMineNeighbor()
{
    return MineNeighbor;
}

// Add 1 mina neighboard to the button
void SButtonGrid::AddMinaNeighbor()
{
    MineNeighbor++;
}

void SButtonGrid::ResetButton()
{
    bIsMine = false;
    bGameOver = false;
    bIsClicked = false;
    MineNeighbor = 0;

}

void SButtonGrid::SetGameOver()
{
    bGameOver = true;
    bIsClicked = true;

    SetButtonTransparency();
    GetTextOnButton();
}
