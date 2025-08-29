// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonGrid.h"
#include "Widgets/Text/STextBlock.h"
#include "Misc/MessageDialog.h"

void SButtonGrid::Construct(const FArguments& InArgs)
{
    bIsMine = InArgs._bIsMine;
    bVisible = InArgs._bVisible;
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

    OnMineClicked = InArgs._OnMineClicked;
    OnCellClicked = InArgs._OnCellClicked;
}

void SButtonGrid::Initialize()
{
    ResetButton();
}

FReply SButtonGrid::OnButtonClicked()
{
    UE_LOG(LogTemp, Error, TEXT("1 button clicked at index: %d"), CellIndex);

    if (bGameOver || bIsClicked)
        return FReply::Handled();

    if (bIsMine)
    {
        bGameOver = true;
        OnMineClicked.ExecuteIfBound();

        FText Message = FText::FromString("YOU LOSE! Sorry");
        FMessageDialog::Open(EAppMsgType::Ok, Message, FText::FromString(TEXT("Game Over")));

        UE_LOG(LogTemp, Error, TEXT("Game Over! The mine has exploded"));
    }
    else
    {
        OnCellClicked.ExecuteIfBound(CellIndex);
    }

    bIsClicked = true;
    bVisible = false;

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
    bIsClicked = false;
    bIsMine = false;
    bGameOver = false;
    bVisible = true;
    MineNeighbor = 0;
}

void SButtonGrid::SetGameOver()
{
    UE_LOG(LogTemp, Error, TEXT("Game Over! Mina exploses -> change color"));
    bGameOver = true;
    bIsClicked = true;

    SetButtonTransparency();
    GetTextOnButton();
}

//void FMinesweeperLevel::ClickButton(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
//{
//    if (InButton.IsValid())
//    {
//        if (InButton->IsEnabled())
//        {
//            if (InButton->HasMine)
//            {
//                // Handle mine button click
//                MineButtonClicked(InButton, InIndex);
//            }
//            else
//            {
//                // Handle free button click
//                FreeButtonClicked(InButton, InIndex);
//
//                // Check for win condition
//                if (AvailableCellsCounter <= Mines)
//                {
//                    int32 Size = GetLevelSize();
//
//                    // Reveal all mines
//                    for (int32 Index : MinesIndexes)
//                    {
//                        // Ensure index is within bounds
//                        if (Index < Size)
//                        {
//                            // Reveal mine button
//                            SlotButtons[Index]->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.WinMineColor")));
//                            SlotButtons[Index]->EnableImage(true);
//                        }
//                    }
//                    // Trigger win event
//                    OnMinesweeperEnd.ExecuteIfBound(true);
//                }
//            }
//        }
//    }
//}

//// Handle mine button click
//void FMinesweeperLevel::MineButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
//{
//	int32 Size = GetLevelSize();
//
//	// Reveal clicked mine
//	InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HasMineColor")));
//	InButton->EnableImage(true);
//
//	// Reveal all other mines
//	for (int32 Index : MinesIndexes)
//	{
//		// Ensure index is within bounds and not the clicked mine
//		if (Index != InIndex)
//		{
//			// Ensure index is within bounds
//			if (Index < Size)
//			{
//				// Reveal mine button
//				SlotButtons[Index]->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.HasMineColor")));
//				SlotButtons[Index]->EnableImage(true);
//			}
//		}
//	}
//
//	// Trigger lose event
//	OnMinesweeperEnd.ExecuteIfBound(false);
//}

//// Handle free button click
//void SButtonGrid::FreeButtonClicked(TSharedPtr<SMinesweeperButton> InButton, const int32 InIndex)
//{
//	InButton->SetEnabled(false);
//
//	// If the button has adjacent mines, display the count
//	if (InButton->AdjacentMinesCounter > 0)
//	{
//		// Display the number of adjacent mines
//		FString Msg = FString::Printf(TEXT("%i"), InButton->AdjacentMinesCounter);
//		InButton->EnableTextBlock(true, FText::FromString(Msg));
//
//		// Set text color based on the number of adjacent mines
//		FString ColorString = TEXT("Minesweeper.MineColor_") + Msg;
//		InButton->SetForegroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.DisabledColor")));
//		InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(*ColorString));
//	}
//	else // No adjacent mines
//	{
//		// If no adjacent mines, reveal neighboring buttons
//		InButton->SetBorderBackgroundColor(FMinesweeperStyleSet::Get().GetSlateColor(TEXT("Minesweeper.DisabledColor")));
//
//		// Calculate row and column from index
//		int32 Row = InIndex / GridCols;
//		int32 Col = InIndex % GridCols;
//
//		// Reveal neighboring buttons (up, down, left, right)
//		TSharedPtr<SMinesweeperButton> NeighbourButton = nullptr;
//		int32 NeighbourIndex = InIndex;
//
//		// Check and reveal the button below
//		if (Row + 1 < GridRows)
//		{
//			// Calculate index of the button below
//			NeighbourIndex = (Row + 1) * GridCols + Col;
//			// Get the button below
//			NeighbourButton = GetButtonAtIndex(Row + 1, Col);
//			// Recursively click the button below
//			ClickButton(NeighbourButton, NeighbourIndex);
//		}
//
//		// Check and reveal the button above
//		if (Row - 1 >= 0)
//		{
//			// Calculate index of the button above
//			NeighbourIndex = (Row - 1) * GridCols + Col;
//			// Get the button above
//			NeighbourButton = GetButtonAtIndex(Row - 1, Col);
//			// Recursively click the button above
//			ClickButton(NeighbourButton, NeighbourIndex);
//		}
//
//		// Check and reveal the button to the right
//		if (Col + 1 < GridCols)
//		{
//			// Calculate index of the button to the right
//			NeighbourIndex = Row * GridCols + (Col + 1);
//			// Get the button to the right
//			NeighbourButton = GetButtonAtIndex(Row, Col + 1);
//			// Recursively click the button to the right
//			ClickButton(NeighbourButton, NeighbourIndex);
//		}
//
//		// Check and reveal the button to the left
//		if (Col - 1 >= 0)
//		{
//			// Calculate index of the button to the left
//			NeighbourIndex = Row * GridCols + (Col - 1);
//			// Get the button to the left
//			NeighbourButton = GetButtonAtIndex(Row, Col - 1);
//			// Recursively click the button to the left
//			ClickButton(NeighbourButton, NeighbourIndex);
//		}
//	}
//	// Decrement available cells counter
//	AvailableCellsCounter--;
//}