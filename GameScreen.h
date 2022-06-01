#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "Snake.h"
#include "Fruit.h"

namespace sfSnake
{
class GameScreen : public Screen
{
public:
	GameScreen(sf::Color backgroundcolor=sf::Color::White,sf::Color linescolor=sf::Color::Black,bool idt=false);

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	std::string picture;
	void generateFruit();
	bool lines;
	sf::Color backgroundcolor;
	sf::Color linecolor;
private:
	Snake snake_;
	std::vector<Fruit> fruit_;
	sf::Clock clock;
};
}

#endif