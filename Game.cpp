#include <SFML/Graphics.hpp>

#include <memory>

#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

sf::Time Game::TimePerFrame = sf::seconds(1.f / 10.f);

std::unique_ptr<Screen> Game::Screen = std::make_unique<MenuScreen>();

Game::Game()
: window_(sf::VideoMode(Game::Width, Game::Height), "sfSnake")
{
	bgMusic_.openFromFile("Music/BGM.wav");
	bgMusic_.setLoop(true);
	bgMusic_.setVolume(80);
	bgMusic_.play();
}

void Game::handleInput()
{
	sf::Event event;

	while (window_.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window_.close();
	}

	Game::Screen->handleInput(window_);
}

void Game::update(sf::Time delta)
{
	Game::Screen->update(delta);
}

void Game::render()
{
	window_.clear();
	Game::Screen->render(window_);
	window_.display();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window_.isOpen())
	{
		sf::Time delta = clock.restart();
		timeSinceLastUpdate += delta;

		while (timeSinceLastUpdate > Game::TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			handleInput();
			update(TimePerFrame);
		}

		render();
	}
}