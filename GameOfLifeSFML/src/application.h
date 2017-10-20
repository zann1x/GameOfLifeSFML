#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>

#include "definitions.h"

#include "config.h"
#include "board.h"
#include "cell.h"

class Application
{
private:
	sf::RenderWindow m_window;

	const Config m_config;
	Board m_board;
	State m_state;

	bool m_state_changed;
	
	const std::string m_title_creation = "Game of Life - Creation mode";
	const std::string m_title_simulation = "Game of Life";

	std::vector<Cell> m_cells;

public:
	Application(const Config& config);

	void run();
private:
	void initWorld();

	void handleEvents();
	void getMouseInput();

	int getCellIndex(int x, int y) const;
	int getCellIndex(const Cell& cell) const;
	Cell& getCell(int x, int y);

	bool cellIsAlive(int x, int y);

	int countNeighBoursOfCellAtPos(int x, int y);
	int countNeighBoursOfCell(const Cell& cell);

	void prepareUpdate();
	void finalizeUpdate();
	void updateWorld();
};