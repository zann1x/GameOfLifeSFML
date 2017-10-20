#include "application.h"

Application::Application(const Config& config)
	: m_window(sf::VideoMode(config.windowWidth, config.windowHeight), "Game of Life"),
	  m_config(config),
	  m_board(config)
{
	Cell cell;
	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			cell.setPosition(x, y);
#if DEBUG
			cell.setState(CellState::DEAD);
#else
			cell.setRandomState();
#endif
			m_cells.push_back(cell);
			m_board.updateQuadOf(cell);
		}
	}

#if DEBUG
	// Manual setup of the glider for testing purposes
	Cell& c1 = getCell(0, 1);
	c1.setState(CellState::ALIVE);
	m_board.updateQuadOf(c1);

	Cell& c2 = getCell(1, 2);
	c2.setState(CellState::ALIVE);
	m_board.updateQuadOf(c2);

	Cell& c3 = getCell(2, 0);
	c3.setState(CellState::ALIVE);
	m_board.updateQuadOf(c3);

	Cell& c4 = getCell(2, 1);
	c4.setState(CellState::ALIVE);
	m_board.updateQuadOf(c4);

	Cell& c5 = getCell(2, 2);
	c5.setState(CellState::ALIVE);
	m_board.updateQuadOf(c5);
#endif
}

void Application::run()
{
	m_window.clear(sf::Color::Blue);

	m_board.draw(m_window);
	m_window.display();
	handleEvents();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (m_window.isOpen())
	{
		m_window.clear(sf::Color::Blue);

		updateWorld();
		m_board.draw(m_window);

		m_window.display();

		handleEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void Application::handleEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}

int Application::getCellIndex(int x, int y) const
{
	return (x * m_config.simulationHeight + y);
}

int Application::getCellIndex(const Cell& cell) const
{
	return getCellIndex(cell.getX(), cell.getY());
}

Cell& Application::getCell(int x, int y)
{
	return m_cells.at(getCellIndex(x, y));
}

bool Application::cellIsAlive(int x, int y)
{
	return getCell(x, y).isAlive();
}

int Application::countNeighBoursOfCellAtPos(int x, int y)
{
	int neighbourCount = 0;

	if (x != 0 && y != 0)
	{
		if (cellIsAlive(x - 1, y - 1)) // upper left
			neighbourCount++;
	}
	if (y != 0)
	{
		if (cellIsAlive(x, y - 1)) // upper mid
			neighbourCount++;
	}
	if (x != m_config.simulationWidth - 1 && y != 0)
	{
		if (cellIsAlive(x + 1, y - 1)) // upper right
			neighbourCount++;
	}

	if (x != 0)
	{
		if (cellIsAlive(x - 1, y)) // left
			neighbourCount++;
	}
	if (x != m_config.simulationWidth - 1)
	{
		if (cellIsAlive(x + 1, y)) // right
			neighbourCount++;
	}

	if (x != 0 && y != m_config.simulationHeight - 1)
	{
		if (cellIsAlive(x - 1, y + 1)) // lower left
			neighbourCount++;
	}
	if (y != m_config.simulationHeight - 1)
	{
		if (cellIsAlive(x, y + 1)) // lower mid
			neighbourCount++;
	}
	if (x != m_config.simulationWidth - 1 && y != m_config.simulationHeight - 1)
	{
		if (cellIsAlive(x + 1, y + 1)) // lower right
			neighbourCount++;
	}

	return neighbourCount;
}

int Application::countNeighBoursOfCell(const Cell& cell)
{
	return countNeighBoursOfCellAtPos(cell.getX(), cell.getY());
}

void Application::prepareUpdate()
{
	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			Cell &cell = getCell(x, y);
			int neighbourCount = countNeighBoursOfCell(cell);

			switch (neighbourCount)
			{
			case 2: // enough neighbours to stay alive
				if (!cell.isAlive())
					break;
			case 3: // enough neighbours to stay alive or come alive again
				cell.setNextState(CellState::ALIVE);
				break;
			default: // too few or too many neighbours
				cell.setNextState(CellState::DEAD);
				break;
			}
		}
	}
}

void Application::finalizeUpdate()
{
	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			Cell& cell = getCell(x, y);
			cell.updateState();
			m_board.updateQuadOf(cell);
		}
	}
}

void Application::updateWorld()
{
	prepareUpdate();
	finalizeUpdate();
}