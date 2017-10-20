#include "board.h"

Board::Board(const Config& config)
	: m_config(config)
{
	sf::RectangleShape quad;

	for (int x = 0; x < m_config.simulationWidth; x++)
	{
		for (int y = 0; y < m_config.simulationHeight; y++)
		{
			quad.setPosition(x * m_config.quadSize, y * m_config.quadSize);
			quad.setSize(sf::Vector2f(m_config.quadSize, m_config.quadSize));
			quad.setFillColor(sf::Color::Red);			
			quad.setOutlineColor(sf::Color::Green);
			m_quads.push_back(quad);
		}
	}
}

int Board::getQuadIndex(int x, int y) const
{
	return (x * m_config.simulationHeight + y);
}

void Board::updateQuadOf(Cell& cell)
{
	int quadIndex = getQuadIndex(cell.getX(), cell.getY());
	m_quads.at(quadIndex).setFillColor((cell.isAlive() ? sf::Color::White : sf::Color::Black));
}

void Board::draw(sf::RenderWindow& window)
{
	for (sf::RectangleShape& quad : m_quads)
	{
		window.draw(quad);
	}
}