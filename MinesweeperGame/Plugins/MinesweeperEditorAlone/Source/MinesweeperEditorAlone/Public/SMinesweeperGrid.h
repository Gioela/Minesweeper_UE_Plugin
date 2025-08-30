// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <ButtonGrid.h>
#include <MinesweeperLevel.h>

/**
 * 
 */
class MINESWEEPEREDITORALONE_API SMinesweeperGrid : public SCompoundWidget
{
public:
	SMinesweeperGrid();
	~SMinesweeperGrid();

public:
    SLATE_BEGIN_ARGS(SMinesweeperGrid)
        : _Rows(3),
        _Columns(3),
        _Mine(2)
        {}
        SLATE_ARGUMENT(int32, Rows)
        SLATE_ARGUMENT(int32, Columns)
        SLATE_ARGUMENT(int32, Mine)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    void Initialize(int32 InRows, int32 InColumns, int32 InMine);

private:
    int32 Rows;
    int32 Cols;
    int32 Mine;
    int32 AvailableButtons;

    TSharedPtr<SGridPanel> GridPanel;

    TArray<TSharedRef<SButtonGrid>> Cells;

    // set of mine's position
    TSet<int32> MineSetIndex;

private:

    TSharedPtr<SButtonGrid> GetCellAtIndex(const int32 InRow, const int32 InCol) const;

    void GenerateGrid(int32 InRows, int32 InColumns, int32 InMine);
        
    void CalculateNeighbords();
    void InitializeMineSetIndex(int32 InMine);

    void RevealEmptyCells(int32 Index);

    void OnCellClicked(int32 Index);
    void OnEndGame();

    void ResetAllProperties();
};
