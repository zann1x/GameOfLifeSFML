#pragma once

#include <random>

#include "definitions.h"

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
	/* setNextState() together with updateState() should be used preferably instead of setState() directly
		as it could happen that when you're in a rendering loop not all cells will be drawn correctly. */
	void setNextState(CellState nextState);
	void setState(CellState state);

	bool isAlive() const;
	void updateState();
};