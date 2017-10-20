#include "cell.h"

Cell::Cell(int x, int y)
	: m_x(x), m_y(y)
{
	m_state = (CellState)(rand() % 2);
}

Cell::Cell(int x, int y, CellState state)
	: m_x(x), m_y(y), m_state(state)
{
}

CellState Cell::getState() const
{
	return m_state;
}

int Cell::getX() const
{
	return m_x;
}

int Cell::getY() const
{
	return m_y;
}

void Cell::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void Cell::setRandomState()
{
	m_state = (CellState)(rand() % 2);
}

void Cell::setNextState(CellState nextState)
{
	m_nextState = nextState;
}

#if DEBUG
void Cell::setState(CellState state)
{
	m_state = state;
}
#endif

bool Cell::isAlive() const
{
	return (m_state == CellState::ALIVE) ? true : false;
}

void Cell::updateState()
{
	m_state = m_nextState;
}