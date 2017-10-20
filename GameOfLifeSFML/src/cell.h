#pragma once

#include <random>

#include "definitions.h"

enum CellState
{
	DEAD,
	ALIVE
};

class Cell
{
private:
	CellState m_state;
	CellState m_nextState;
	int m_x;
	int m_y;

public:
	Cell() = default;
	Cell(int x, int y);
	Cell(int x, int y, CellState state);
	
	CellState getState() const;
	int getX() const;
	int getY() const;

	void setPosition(int x, int y);
	void setRandomState();
	void setNextState(CellState nextState);
#if DEBUG
	void setState(CellState state);
#endif

	bool isAlive() const;
	void updateState();
};