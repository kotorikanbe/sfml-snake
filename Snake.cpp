#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <cmath>
#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;
const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680;
const int Snake::InitialSize = 5;
static float frad(float rad)
{
	// if (2.9 >= rad && rad >= 0.5)
		return rad / PI * 180.0;
	// if (-2.9 <= rad && rad < -0.5)
	// 	return 360 - (-rad) / PI * 180;
	// if (-0.5 <= rad && rad < 0.5)
	// 	return 0.0;
	// else
	// 	return 180;
}
Snake::Snake() : direction_(sf::Vector2f(0.0, 0.15)), hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.aiff");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);
}

void Snake::initNodes()
{
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		if (i == 0)
		{
			SnakeNode snakenode(sf::Vector2f(Game::Width / 2 - SnakeNode::Width / 2, Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)), true);
			nodes_.push_back(snakenode);
		}
		else
		{
			SnakeNode snakenode(sf::Vector2f(Game::Width / 2 - SnakeNode::Width / 2, Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i)), false);
			nodes_.push_back(snakenode);
		}
	}
}
// void Snake::initNodes()
// {
// 	for (int i = 0; i < Snake::InitialSize; ++i)
// 	{
// 		nodes_.push_back(SnakeNode(sf::Vector2f(
// 			Game::Width / 2 - SnakeNode::Width / 2,
// 			Game::Height / 2 - (SnakeNode::Height / 2) + (SnakeNode::Height * i))));
// 	}
// }

// void Snake::handleInput()
// {
// 	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
// 		direction_ = Direction::Up;
// 	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
// 		direction_ = Direction::Down;
// 	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
// 		direction_ = Direction::Left;
// 	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
// 		direction_ = Direction::Right;
// }

void Snake::handleInput(sf::RenderWindow &window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f start = nodes_[0].getPosition();
		sf::Vector2i end = sf::Mouse::getPosition(window);
		sf::Vector2f vector(static_cast<float>(end.x - start.x), static_cast<float>(end.y - start.y));
		float tmp = std::atan2(vector.y, vector.x);
		vector.x = vector.x /sqrt(pow(vector.x,2)+pow(vector.y,2)) ;
		vector.y = vector.y /sqrt(pow(vector.x,2)+pow(vector.y,2)) ;
		direction_ = std::move(vector);
		arc_ = frad(tmp);
	}
}
void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit> &fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();

	for (auto it = fruits.begin(); it != fruits.end(); ++it)
	{
		if (it->getBounds().intersects(nodes_[0].getBounds()))
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		grow();
		fruits.erase(toRemove);
	}
}

void Snake::grow()
{
	// switch (direction_)
	// {
	// case Direction::Up:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
	// 											nodes_[nodes_.size() - 1].getPosition().y + SnakeNode::Height)));
	// 	break;
	// case Direction::Down:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
	// 											nodes_[nodes_.size() - 1].getPosition().y - SnakeNode::Height)));
	// 	break;
	// case Direction::Left:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + SnakeNode::Width,
	// 											nodes_[nodes_.size() - 1].getPosition().y)));
	// 	break;
	// case Direction::Right:
	// 	nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x - SnakeNode::Width,
	// 											nodes_[nodes_.size() - 1].getPosition().y)));
	// 	break;
	// }
	SnakeNode snakenode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x * SnakeNode::Width, nodes_[nodes_.size() - 1].getPosition().y + direction_.y * SnakeNode::Height), false, nodes_[nodes_.size() - 1].arc_);
	nodes_.push_back(snakenode);
}

unsigned Snake::getSize() const
{
	return nodes_.size();
}

bool Snake::hitSelf() const
{
	return hitSelf_;
}

void Snake::checkSelfCollisions()
{
	// SnakeNode &headNode = nodes_[0];

	// for (decltype(nodes_.size()) i = 1; i < nodes_.size(); ++i)
	// {
	// 	if (headNode.getBounds().intersects(nodes_[i].getBounds()))
	// 	{
	// 		dieSound_.play();
	// 		sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
	// 		hitSelf_ = true;
	// 	}
	// }
}

void Snake::checkEdgeCollisions()
{
	SnakeNode &headNode = nodes_[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move()
{
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
	
			nodes_[i].setPosition(nodes_.at(i - 1).getPosition());
		
		nodes_[i].arc_ = nodes_.at(i - 1).arc_;
	}

	// switch (direction_)
	// {
	// case Direction::Up:
	// 	nodes_[0].move(0, -SnakeNode::Height);
	// 	break;
	// case Direction::Down:
	// 	nodes_[0].move(0, SnakeNode::Height);
	// 	break;
	// case Direction::Left:
	// 	nodes_[0].move(-SnakeNode::Width, 0);
	// 	break;
	// case Direction::Right:
	// 	nodes_[0].move(SnakeNode::Width, 0);
	// 	break;
	// }
	nodes_[0].move(direction_.x * SnakeNode::Width , direction_.y * SnakeNode::Height);
	nodes_[0].arc_ = arc_;
}

void Snake::render(sf::RenderWindow &window)
{
	for (auto node : nodes_)
		node.render(window);
}