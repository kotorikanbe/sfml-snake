#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen(sf::Color backgroundscolor,sf::Color linescolor ,bool idt) : lines(idt),backgroundcolor(backgroundscolor),linecolor(linescolor),snake_()
{

}

void GameScreen::handleInput(sf::RenderWindow &window)
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> Distribution(0, 100000);
	snake_.checkFruitCollisions(fruit_);
	snake_.checkSelfCollisions();
	if (Distribution(engine) % 200 == 0)
		generateFruit();
	snake_.handleInput(window,fruit_);
	snake_.checkFruitCollisions(fruit_);
	snake_.checkSelfCollisions();
}

void GameScreen::update(sf::Time delta)
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> Distribution(0, 100000);
	snake_.checkFruitCollisions(fruit_);
	snake_.checkSelfCollisions();
	if (Distribution(engine) % 200 == 0)
		generateFruit();
	snake_.checkFruitCollisions(fruit_);
	snake_.checkSelfCollisions();
	snake_.update(delta,fruit_);
	
	snake_.checkFruitCollisions(fruit_);
	snake_.checkSelfCollisions();

	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize());
	snake_.checkFruitCollisions(fruit_);
}

void GameScreen::render(sf::RenderWindow &window)
{
	sf::RectangleShape background(sf::Vector2f(Game::Width,Game::Height));
	background.setFillColor(backgroundcolor);
	window.draw(background);
	if(lines){
		for(int i=0;i<Game::Width;i+=10){
			sf::RectangleShape cline(sf::Vector2f(2.0,Game::Height));
			cline.setFillColor(linecolor);
			cline.setPosition(sf::Vector2f(static_cast<float>(i),0));
			window.draw(cline);
		}
		for(int i=0;i<Game::Height;i+=10){
			sf::RectangleShape cline(sf::Vector2f(Game::Width,2.0));
			cline.setFillColor(linecolor);
			cline.setPosition(sf::Vector2f(0,static_cast<float>(i)));
			window.draw(cline);
		}
	}
	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);
}

void GameScreen::generateFruit()
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> xDistribution(0, Game::Width - SnakeNode::Width);
	static std::uniform_int_distribution<int> yDistribution(0, Game::Height - SnakeNode::Height);

	fruit_.push_back(Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine))));
}
