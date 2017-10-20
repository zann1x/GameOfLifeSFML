#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "config.h"
#include "cell.h"

class Board
{
private:
	const Config m_config;
	std::vector<sf::RectangleShape> m_quads;

public:
	Board(const Config& config);

	int getQuadIndex(int x, int y) const;

	void updateQuadOf(Cell& cell);
	void draw(sf::RenderWindow& window);
};