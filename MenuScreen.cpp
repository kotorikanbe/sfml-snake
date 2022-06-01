#include <SFML/Graphics.hpp>

#include <memory>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"
#include "GameOverScreen.h"
using namespace sfSnake;
static sf::Color Brown(139, 69, 19, 255);
MenuScreen::MenuScreen()
{
	font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit");

	snakeText_.setFont(font_);
	snakeText_.setString("Snake!");
	snakeText_.setFillColor(sf::Color::Green);
	snakeText_.setCharacterSize(64);
	snakeText_.setStyle(sf::Text::Bold);
	backgroundcolor = sf::Color::White;
	linescolor = sf::Color::Black;
	lines = false;
	change.setFont(font_);
	change.setString("Press [A] to change background color\n\nPress [L] to import lines\n\n");
	change.setFillColor(sf::Color::White);
	change.setCharacterSize(15);
	sf::FloatRect changeBounds = change.getLocalBounds();
	change.setOrigin(changeBounds.left + changeBounds.width / 2,
					 changeBounds.top + changeBounds.height / 2);
	change.setPosition(Game::Width / 2.0, Game::Height * 4.0 / 5.0);
	sf::FloatRect textBounds = text_.getLocalBounds();
	display1.setFont(font_);
	display1.setString("BACKGROUND:WHITE");
	display1.setFillColor(sf::Color::White);
	display1.setCharacterSize(15);
	sf::FloatRect display1Bounds = display1.getLocalBounds();
	display1.setOrigin(display1Bounds.left + display1Bounds.width / 2,
					   display1Bounds.top + display1Bounds.height / 2);
	display1.setPosition(Game::Width / 2.0, Game::Height * 0.9);
	display2.setFont(font_);
	display2.setString("LINES:\t OFF");
	display2.setFillColor(sf::Color::White);
	display2.setCharacterSize(15);
	sf::FloatRect display2Bounds = display2.getLocalBounds();
	display2.setOrigin(display2Bounds.left + display2Bounds.width / 2,
					   display2Bounds.top + display2Bounds.height / 2);
	display2.setPosition(Game::Width / 2.0, Game::Height * 0.925);
	display3.setFont(font_);
	display3.setString("LINES:BLACK");
	display3.setFillColor(sf::Color::Black);
	display3.setCharacterSize(15);
	sf::FloatRect display3Bounds = display3.getLocalBounds();
	display3.setOrigin(display3Bounds.left + display3Bounds.width / 2,
					   display3Bounds.top + display3Bounds.height / 2);
	display3.setPosition(Game::Width / 2.0, Game::Height * 0.95);
	text_.setOrigin(textBounds.left + textBounds.width / 2,
					textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::Width / 2, Game::Height / 2);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
						 snakeTextBounds.top + snakeTextBounds.height / 2);
	snakeText_.setPosition(Game::Width / 2, Game::Height / 4);
}

void MenuScreen::handleInput(sf::RenderWindow &window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (backgroundcolor == sf::Color::White)
		{

			backgroundcolor = sf::Color::Black;
			display1.setString("BACKGROUND:BLACK");
			display1.setFillColor(sf::Color::Black);
		}

		else if (backgroundcolor == sf::Color::Black)
		{
			backgroundcolor = Brown;
			display1.setString("BACKGROUND:BROWN");
			display1.setFillColor(Brown);
		}
		else if (backgroundcolor == Brown)
		{
			backgroundcolor = sf::Color::White;
			display1.setString("BACKGROUND:WHITE");
			display1.setFillColor(sf::Color::White);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		Game::TimePerFrame=sf::seconds(1.0f/100.0f);
		Game::Screen = std::make_shared<GameScreen>(backgroundcolor, linescolor, lines);
		}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
		if(lines){
			lines=false;
			display2.setString("LINES:\t OFF");
			change.setString("Press [A] to change background color\n\nPress [L] to import lines\n\n");
		}
		else{
			lines=true;
			display2.setString("LINES:\t ON");
			change.setString("Press [A] to change background color\n\nPress [L] to import lines\n\nPress [s] to change lines color");
		}
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)&& lines){
		if (linescolor == sf::Color::White)
		{

			linescolor = sf::Color::Black;
			display3.setString("LINES:BLACK");
			display3.setFillColor(sf::Color::Black);
		}

		else if (linescolor == sf::Color::Black)
		{
			linescolor = Brown;
			display3.setString("LINES:BROWN");
			display3.setFillColor(Brown);
		}
		else if (linescolor == Brown)
		{
			linescolor = sf::Color::White;
			display3.setString("LINES:WHITE");
			display3.setFillColor(sf::Color::White);
		}
	}
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;
	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}
}

void MenuScreen::render(sf::RenderWindow &window)
{
	sf::Color orange(255, 165, 0, 255);
	sf::Color Cyan(0, 255, 255, 200);
	sf::Color purple(160, 32, 240, 255);
	sf::RectangleShape shape(sf::Vector2f(Game::Width, Game::Height / 7.0));
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(0.0, 0.0);
	window.draw(shape);
	shape.setFillColor(orange);
	shape.setPosition(0.0, Game::Height / 7.0);
	window.draw(shape);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(0.0, Game::Height * 2.0 / 7.0);
	window.draw(shape);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(0.0, Game::Height * 3.0 / 7.0);
	window.draw(shape);
	shape.setFillColor(Cyan);
	shape.setPosition(0.0, Game::Height * 4.0 / 7.0);
	window.draw(shape);
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(0.0, Game::Height * 5.0 / 7.0);
	window.draw(shape);
	shape.setFillColor(purple);
	shape.setPosition(0.0, Game::Height * 6.0 / 7.0);
	window.draw(shape);
	window.draw(text_);
	window.draw(snakeText_);
	window.draw(change);
	window.draw(display1);
	window.draw(display2);
	if(lines){
		window.draw(display3);
	}
}