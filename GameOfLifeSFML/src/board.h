#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "definitions.h"

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
	int getQuadIndex(sf::RectangleShape& quad) const;
	sf::RectangleShape& getQuad(int x, int y);

	void updateQuadOf(Cell& cell);
	void draw(sf::RenderWindow& window);
};