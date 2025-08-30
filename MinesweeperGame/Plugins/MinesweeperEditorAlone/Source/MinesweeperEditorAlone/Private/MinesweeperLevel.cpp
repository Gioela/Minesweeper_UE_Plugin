// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperLevel.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

DEFINE_LOG_CATEGORY(MinesweeperLevLog);

MinesweeperLevel::MinesweeperLevel(const int32 InRows, const int32 InColumns, const int32 InMines)
{
	Rows = InRows;
	Columns = InColumns;
	Mines = InMines;

	MinesweeperGridWidget = SNew(SMinesweeperGrid);
	MinesweeperGridWidget->Initialize(Rows, Columns, Mines);

	UE_LOG(MinesweeperLevLog, Warning, TEXT("Level created with %d rows, %d columns, and %d mines."), Rows, Columns, Mines);
}

MinesweeperLevel::~MinesweeperLevel()
{
}

int32 MinesweeperLevel::GetRows() const
{
	return Rows;
}

int32 MinesweeperLevel::GetColumns() const
{
	return Columns;
}

int32 MinesweeperLevel::GetMines() const
{
	return Mines;
}

int32 MinesweeperLevel::GetSize() const
{
	return Rows * Columns;
}

void MinesweeperLevel::SetRows(const int32 InRows)
{
	Rows = InRows;
}

void MinesweeperLevel::SetColumns(const int32 InColumns)
{
	Columns = InColumns;
}

void MinesweeperLevel::SetMines(const int32 InMines)
{
	Mines = InMines;
}

void MinesweeperLevel::UpdateLevelDetails(const int32& InRows, const int32& InColumns, const int32& InMine)
{
	Rows = InRows;
	Columns = InColumns;
	Mines = InMine;
}

void MinesweeperLevel::GenerateGrid()
{
	MinesweeperGridWidget->Initialize(Rows, Columns, Mines);
}
