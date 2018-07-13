#include "application.h"

Application::Application(const Config& config)
	: m_window(sf::VideoMode(config.windowWidth, config.windowHeight), "Game of Life"),
	  m_config(config),
	  m_board(config),
	  m_state(State::CREATION),
	  m_state_changed(true)
{
	if (m_config.randomWorldCreation)
	{
		m_state = State::SIMULATION;
	}

	Cell cell;
	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			cell.setPosition(x, y);

			if (config.randomWorldCreation == true)
				cell.setRandomState();
			else
				cell.setState(CellState::DEAD);

			m_cells.push_back(cell);
			m_board.updateQuadOf(cell);
		}
	}
}

void Application::run()
{
	m_window.clear(sf::Color::Blue);
	m_board.draw(m_window);
	m_window.display();
	handleEvents();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	m_window.requestFocus();

	while (m_window.isOpen())
	{
		m_window.clear(sf::Color::Blue);

		switch (m_state)
		{
		case State::CREATION:
			if (m_state_changed)
			{
				m_state_changed = false;
				m_window.setTitle(m_title_creation);
			}

			getMouseInput();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				m_state = State::SIMULATION;
				m_state_changed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				initWorld();
			}
			break;
		case State::SIMULATION:
			if (m_state_changed)
			{
				m_state_changed = false;
				m_window.setTitle(m_title_simulation);
			}

			updateWorld();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_state = State::CREATION;
				m_state_changed = true;
			}
			break;
		}

		m_board.draw(m_window);
		m_window.display();

		handleEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void Application::initWorld()
{
	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			Cell& cell = getCell(x, y);
			cell.setState(CellState::DEAD);
			m_board.updateQuadOf(cell);
		}
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

void Application::getMouseInput()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
		return;

	sf::Vector2i mousePositionInWindow = sf::Mouse::getPosition(m_window);
	if (mousePositionInWindow.x < 0 || mousePositionInWindow.x > m_window.getSize().x ||
		mousePositionInWindow.y < 0 || mousePositionInWindow.y > m_window.getSize().y)
	{
		return;
	}

	int mouseXPositionInQuad = std::ceil(mousePositionInWindow.x / m_config.quadSize);
	int mouseYPositionInQuad = std::ceil(mousePositionInWindow.y / m_config.quadSize);

	Cell& cell = getCell(mouseXPositionInQuad, mouseYPositionInQuad);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!cell.isAlive())
		{
			cell.setState(CellState::ALIVE);
			m_board.updateQuadOf(cell);
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (cell.isAlive())
		{
			cell.setState(CellState::DEAD);
			m_board.updateQuadOf(cell);
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