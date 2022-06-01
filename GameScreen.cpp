#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"

using namespace sfSnake;

GameScreen::GameScreen() : snake_()
{
	picture="Pictures/while_full.png";
}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> Distribution(0, 100000);
	if (fruit_.size() <= 5&& Distribution(engine)%10==0)
		generateFruit();
	snake_.handleInput(window);

}

void GameScreen::update(sf::Time delta)
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> Distribution(0, 100000);
	if (fruit_.size() <= 5&& Distribution(engine)%10==0)
		generateFruit();

	snake_.update(delta);
	snake_.checkFruitCollisions(fruit_);

	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize());
}

void GameScreen::render(sf::RenderWindow& window)
{
	sf::Texture background;
	background.loadFromFile(picture);
	sf::Sprite sprite(background);
	window.draw(sprite);
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

