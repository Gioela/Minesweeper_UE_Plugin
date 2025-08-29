// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <SMinesweeperGrid.h>

class SMinesweeperGrid;
/**
 * 
 */
class MINESWEEPEREDITORALONE_API MinesweeperLevel
{
public:
	MinesweeperLevel(const int32 InRows, const int32 InColumns, const int32 InMines);
	~MinesweeperLevel();

	// getter
	int32 GetRows() const;
	int32 GetColumns() const;
	int32 GetMines() const;
	int32 GetSize() const;

	// setter
	void SetRows(const int32 InRows);
	void SetColumns(const int32 InColumns);
	void SetMines(const int32 InMines);

	void UpdateLevelDetails(const int32& InRows, const int32& InColumns, const int32& InMine);

	void GenerateGrid();
	void GenerateMinesIndexes();

public:
	// Level properties
	int32 Rows;
	int32 Columns;
	int32 Mines;

	// Slate game grid widget
	TSharedPtr<SMinesweeperGrid> MinesweeperGridWidget;

	// Set to store the mine positions
	TSet<int32> MineIndexPositions;

};

DECLARE_LOG_CATEGORY_EXTERN(MinesweeperLevLog, Log, All)
