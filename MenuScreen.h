#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Screen.h"

namespace sfSnake
{
class MenuScreen : public Screen
{
public:
	MenuScreen();

	void handleInput(sf::RenderWindow& window) override;
	void update(sf::Time delta) override;
	void render(sf::RenderWindow& window) override;
	bool lines;
	sf::Color backgroundcolor;
	sf::Color linescolor;
private:
	sf::Font font_;
	sf::Text snakeText_;
	sf::Text text_;
	sf::Text change;
	sf::Text display1;
	sf::Text display2;
	sf::Text display3;
};
}

#endif