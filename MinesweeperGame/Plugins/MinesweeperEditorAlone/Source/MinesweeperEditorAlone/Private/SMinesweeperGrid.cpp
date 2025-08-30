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
    CalculateNeighbords();
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
            }

            TSharedPtr<SButtonGrid> Button;
            SAssignNew(Button, SButtonGrid)
                .bIsMine(bMine)
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
    }
}

void SMinesweeperGrid::OnCellClicked(int32 Index)
{
    // check if index is valid
    if (!Cells.IsValidIndex(Index))
    {
        return;
    }

    TSharedRef<SButtonGrid> Cell = Cells[Index];
    if (Cell->GetIsMine())
    {
        FText Message = FText::FromString("YOU LOSE! Sorry");
        FMessageDialog::Open(EAppMsgType::Ok, Message, FText::FromString(TEXT("Game Over")));

        UE_LOG(LogTemp, Error, TEXT("Game Over! The mine has exploded"));
        OnEndGame();
        return;
    }

    if (Cell->GetMineNeighbor() > 0)
    {
        Cell->SetClicked(true);
        AvailableButtons--;
    }
    else
    {
        RevealEmptyCells(Index);
    }

    if (AvailableButtons < MineSetIndex.Num())
    {
        FText Message = FText::FromString("YOU WIN! Congrats");
        FMessageDialog::Open(EAppMsgType::Ok, Message, FText::FromString(TEXT("The Winner is... YOU!")));

        UE_LOG(LogTemp, Error, TEXT("Player Wins! Congrats"));
        OnEndGame();
    }
}

void SMinesweeperGrid::OnEndGame()
{
    for (TSharedRef<SButtonGrid> Cell : Cells)
    {
        if (!Cell->GetIsClicked())
        {
            Cell->SetGameOver();
        }
    }
}

// Calculate adjacent mine counts
void SMinesweeperGrid::CalculateNeighbords()
{
	if (MineSetIndex.Num() >= 0)
	{
		for (int32 Index : MineSetIndex)
		{
			// Calculate row and column from index
			int32 Row = Index / Cols;
			int32 Col = Index % Cols;

            // Neighbords at the BUTTOM ROW index
			if (Row + 1 < Rows)
			{
                GetCellAtIndex(Row + 1, Col)->AddMinaNeighbor();

                if (Col - 1 >= 0 && Col - 1 < Cols)
                { 
                    GetCellAtIndex(Row + 1, Col - 1)->AddMinaNeighbor();
                }

                if (Col + 1 < Cols)
                {
                    GetCellAtIndex(Row + 1, Col + 1)->AddMinaNeighbor();
                }
			}

            // Neighbords at the UPPER ROW index
            if (Row - 1 >= 0)
            {
                GetCellAtIndex(Row - 1, Col)->AddMinaNeighbor();

                if (Col - 1 >= 0 && Col - 1 < Cols)
                {
                    GetCellAtIndex(Row - 1, Col - 1)->AddMinaNeighbor();
                }

                if (Col + 1 < Cols)
                {
                    GetCellAtIndex(Row - 1, Col + 1)->AddMinaNeighbor();
                }
            }

            // Neighbords at the LEFT COLUM index
            if (Col - 1 >= 0 && Col < Cols)
            {
                GetCellAtIndex(Row, Col - 1)->AddMinaNeighbor();
            }

            // Neighbords at the RIGHT COLUM index
            if (Col + 1 < Cols)
            {
                GetCellAtIndex(Row, Col + 1)->AddMinaNeighbor();
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
    return Cells[Index];
}

// iterate over neighbord cells
void SMinesweeperGrid::RevealEmptyCells(int32 Index)
{
    // index is a valid position in cells array?
    if (!Cells.IsValidIndex(Index))
    {
        return;
    }

    TSharedRef<SButtonGrid> Cell = Cells[Index];

    if (Cell->GetMineNeighbor() > 0)
    {
        Cell->RevealNeighbord();
        AvailableButtons--;
    }

    // cell is: clicked || mine or has neighboard?
    if (Cell->GetIsClicked() || Cell->GetIsMine() || Cell->GetMineNeighbor() > 0)
    {
        return;
    }

    Cell->SetClicked(true);

    int32 Row = Index / Cols;
    int32 Col = Index % Cols;

    for (int32 dRow = -1; dRow <= 1; ++dRow)
    {
        for (int32 dCol = -1; dCol <= 1; ++dCol)
        {
            if (dRow == 0 && dCol == 0) 
            {
                continue;
            }

            int32 NewRow = Row + dRow;
            int32 NewCol = Col + dCol;

            if (NewRow >= 0 && NewRow < Rows && NewCol >= 0 && NewCol < Cols)
            {
                int32 NewIndex = NewRow * Cols + NewCol;
                RevealEmptyCells(NewIndex);
            }
            //else
            //{
            //    UE_LOG(MinesweeperLevLog, Error, TEXT("[SMinesweeperGrid::RevealEmptyCells][ 8] cell i.%d (r.%d c.%d) evaluate cell coord (r.%d c.%d) NO VALID POSITION"), Index, Row, Col, NewRow, NewCol);
            //}
        }
    }
    AvailableButtons--;
    //UE_LOG(MinesweeperLevLog, Error, TEXT("[SMinesweeperGrid::RevealEmptyCells][13] CLOSE index.%d | AvailableButtons are ab.%d"), Index, AvailableButtons);
}