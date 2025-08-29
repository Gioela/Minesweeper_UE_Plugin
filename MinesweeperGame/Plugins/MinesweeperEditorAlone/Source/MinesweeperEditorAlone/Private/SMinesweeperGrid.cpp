// Fill out your copyright notice in the Description page of Project Settings.

#include "SMinesweeperGrid.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

SMinesweeperGrid::SMinesweeperGrid()
{
}

SMinesweeperGrid::~SMinesweeperGrid()
{
}

void SMinesweeperGrid::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SAssignNew(GridPanel, SGridPanel)
        ];

    Rows = InArgs._Rows;
    Cols = InArgs._Columns;
    Mine = InArgs._Mine;

    Initialize(InArgs._Rows, InArgs._Columns, InArgs._Mine);
}

void SMinesweeperGrid::Initialize(int32 InRows, int32 InColumns, int32 InMine)
{
    ResetAllProperties();

    Rows = InRows;
    Cols = InColumns;
    Mine = InMine;
    AvailableButtons = Rows * Cols;

    InitializeMineSetIndex(Mine);
    GenerateGrid(Rows, Cols, Mine);
    AssignNeighbords();
}

void SMinesweeperGrid::GenerateGrid(int32 InRows, int32 InColumns, int32 InMines)
{
    GridPanel->ClearChildren();

    Cells.Empty(InRows * InColumns);

    for (int32 Row = 0; Row < InRows; ++Row)
    {
        for (int32 Col = 0; Col < InColumns; ++Col)
        {
            int32 Index = Row * InColumns + Col;

            bool bMine = false;
            if (MineSetIndex.Contains(Index))
            {
                bMine = true;
                UE_LOG(MinesweeperLevLog, Warning, TEXT("Mina is on r.%d c.%d"), Row, Col);
            }

            TSharedPtr<SButtonGrid> Button;
            SAssignNew(Button, SButtonGrid)
                .bIsMine(bMine)
                .OnMineClicked_Raw(this, &SMinesweeperGrid::OnGameOver)
                .OnCellClicked_Raw(this, &SMinesweeperGrid::OnCellClicked)
                ;
            Button->CellIndex = Index;

            GridPanel->AddSlot(Col, Row)
                [
                    Button.ToSharedRef()
                ];
            Cells.Add(Button.ToSharedRef());
        }
    }
}

// Define the new set of Mine index
// each index corresponding to a mine position in the game grid
void SMinesweeperGrid::InitializeMineSetIndex(int32 InMine)
{
    MineSetIndex.Empty(InMine);

    int32 Size = Rows * Cols;
    int32 NewIndex = -1;

    for (int32 It = 0; It < Mine; It++)
    {
        do
        {
            NewIndex = FMath::RandRange(0, Size - 1);
        } while (MineSetIndex.Contains(NewIndex));
        
        MineSetIndex.Add(NewIndex);
        UE_LOG(MinesweeperLevLog, Warning, TEXT("Mina is at index.%d"), NewIndex);
    }
}

void SMinesweeperGrid::OnCellClicked(int32 Index)
{
    if (!Cells.IsValidIndex(Index)) return;

    TSharedRef<SButtonGrid> Cell = Cells[Index];
    if (Cell->GetIsMine() || Cell->GetMineNeighbor() > 0)
    {
        Cell->SetClicked(true);
        return;
    }

    RevealEmptyCells(Index);
}

void SMinesweeperGrid::OnGameOver()
{
    for (TSharedRef<SButtonGrid> Button : Cells)
    {
        Button->SetGameOver();
    }
}

// Assign mines to buttons and calculate adjacent mine counts
void SMinesweeperGrid::AssignNeighbords()
{
	// Assign mines and calculate adjacent mines for each button
	if (MineSetIndex.Num() >= 0)
	{
		// Reset all buttons
		for (int32 Index : MineSetIndex)
		{
			// Calculate row and column from index
			int32 Row = Index / Rows;
			int32 Col = Index % Cols;

            UE_LOG(MinesweeperLevLog, Warning, TEXT("Index Mina and neighbord is r.%d c.%d"), Row, Col);

			// Increment adjacent mine counters for neighboring buttons
			// Check and increment for the button below
			if (Row + 1 < Rows)
			{
                GetCellAtIndex(Row + 1, Col)->AddMinaNeighbor();
			}

            // below-left
            if (Row + 1 < Rows && Col - 1 >= 0)
            {
                GetCellAtIndex(Row + 1, Col - 1)->AddMinaNeighbor();
            }

            // below-right
            if (Row + 1 < Rows && Col + 1 < Cols)
            {
                GetCellAtIndex(Row + 1, Col + 1)->AddMinaNeighbor();
            }

			// Check and increment for the button above
			if (Row - 1 >= 0)
			{
                GetCellAtIndex(Row - 1, Col)->AddMinaNeighbor();
			}

            // above-left
            if (Row - 1 >= 0 && Col - 1 >= 0)
            {
                GetCellAtIndex(Row - 1, Col - 1)->AddMinaNeighbor();
            }

            // above-right
            if (Row - 1 >= 0 && Col + 1 < Cols)
            {
                GetCellAtIndex(Row - 1, Col + 1)->AddMinaNeighbor();
            }

			// Check and increment for the button to the right
			if (Col + 1 < Cols)
			{
                GetCellAtIndex(Row, Col + 1)->AddMinaNeighbor();
			}

			// Check and increment for the button to the left
			if (Col - 1 >= 0)
			{
                GetCellAtIndex(Row, Col - 1)->AddMinaNeighbor();
			}
		}
	}
    else
    {
        UE_LOG(MinesweeperLevLog, Warning, TEXT("No Mine index found"));
    }
}

void SMinesweeperGrid::ResetAllProperties()
{
    Rows = 0;
    Cols = 0;
    Mine = 0;

    if (Cells.Num() >= 0)
    {
        for (TSharedRef<SButtonGrid> Button : Cells)
        {
            Button->ResetButton();
        }
    }

    if (MineSetIndex.Num() > 0)
    {
        MineSetIndex.Empty();
    }
}

// Get button at specific row and column
TSharedPtr<SButtonGrid> SMinesweeperGrid::GetCellAtIndex(const int32 InRow, const int32 InCol) const
{

    // Calculate index from row and column
    int32 Index = InRow * Cols + InCol;
    UE_LOG(MinesweeperLevLog, Warning, TEXT("GetCellAtIndex r.%d c.%d = Index %d"), InRow, InCol, Index);

    // Validate index
    if (Index < 0 || Index >= Cells.Num())
    {
        return nullptr;
    }
    // Return the button at the calculated index
    return Cells[Index];
}

void SMinesweeperGrid::RevealEmptyCells(int32 Index)
{
    if (!Cells.IsValidIndex(Index)) return;

    TSharedRef<SButtonGrid> Cell = Cells[Index];
    if (Cell->GetIsClicked() || Cell->GetIsMine() || Cell->GetMineNeighbor() > 0)
        return;

    Cell->SetClicked(true);

    int32 Row = Index / Cols;
    int32 Col = Index % Cols;

    for (int32 dRow = -1; dRow <= 1; ++dRow)
    {
        for (int32 dCol = -1; dCol <= 1; ++dCol)
        {
            if (dRow == 0 && dCol == 0) continue;

            int32 NewRow = Row + dRow;
            int32 NewCol = Col + dCol;

            if (NewRow >= 0 && NewRow < Rows && NewCol >= 0 && NewCol < Cols)
            {
                int32 NewIndex = NewRow * Cols + NewCol;
                RevealEmptyCells(NewIndex);
            }
        }
    }
}