#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cmath>
#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"
static sf::Vector2f edge;
using namespace sfSnake;
const float PI = 3.1415926535;
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
static float distance(const sf::Vector2f &dot1, const sf::Vector2f &dot2)
{
	return sqrt(pow(dot1.x - dot2.x, 2) + pow(dot1.y - dot2.y, 2));
}
static void edgecrossing(SnakeNode &headNode)
{
	if (headNode.getPosition().x <= 0)
	{
		edge = headNode.getPosition();
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	}
	else if (headNode.getPosition().x >= Game::Width)
	{
		edge = headNode.getPosition();
		headNode.setPosition(0, headNode.getPosition().y);
	}
	else if (headNode.getPosition().y <= 0)
	{
		edge = headNode.getPosition();
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	}
	else if (headNode.getPosition().y >= Game::Height)
	{
		edge = headNode.getPosition();
		headNode.setPosition(headNode.getPosition().x, 0);
	}
}
Snake::Snake() : hitSelf_(false),direction_(sf::Vector2f(0.0, 1))
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
			SnakeNode snakenode(sf::Vector2f(Game::Width / 2 - SnakeNode::Width / 2, Game::Height / 2 - (SnakeNode::Height / 2) - (SnakeNode::Height * i)), true, 90);
			nodes_.push_back(snakenode);
		}
		else
		{
			SnakeNode snakenode(sf::Vector2f(Game::Width / 2 - SnakeNode::Width / 2, Game::Height / 2 - (SnakeNode::Height / 2) - (SnakeNode::Height * (static_cast<float>(i) + 0.1))), false, 90);
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

void Snake::handleInput(sf::RenderWindow &window, std::vector<Fruit> &fruits)
{
	checkSelfCollisions();
	checkFruitCollisions(fruits);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		checkFruitCollisions(fruits);
		sf::Vector2f start = nodes_[0].getPosition();
		sf::Vector2i end = sf::Mouse::getPosition(window);
		sf::Vector2f vector(static_cast<float>(end.x) - start.x, static_cast<float>(end.y) - start.y);
		float tmp = std::atan2(vector.y, vector.x);
		vector.x = vector.x / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		vector.y = vector.y / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		direction_ = std::move(vector);
		arc_ = frad(tmp);
	}
	checkFruitCollisions(fruits);
	checkSelfCollisions();
}
void Snake::update(sf::Time delta, std::vector<Fruit> &fruits)
{
	checkFruitCollisions(fruits);
	checkEdgeCollisions();
	checkSelfCollisions();
	checkFruitCollisions(fruits);
	checkSelfCollisions();
	move();
}

void Snake::checkFruitCollisions(std::vector<Fruit> &fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();

	for (auto it = fruits.begin(); it != fruits.end(); ++it)
	{
		sf::Vector2f tmp = nodes_[0].getPosition();
		if (distance(tmp,it->getPositions())<15.0f)
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		grow(toRemove->getcolor());
		fruits.erase(toRemove);
	}
}

void Snake::grow(sf::Color color)
{
	static sf::Color brown1{140, 81, 25, 255};
	static sf::Color brown2{139, 105, 20, 255};
	static sf::Color brown3{139, 69, 19, 255};
	static sf::Color brown4{165, 42, 42, 255};
	static sf::Color brown5{205, 149, 12, 255};
	static sf::Color blue1{152, 245, 255, 255};
	static sf::Color blue2{0, 0, 255, 255};
	static sf::Color blue3{135, 206, 250, 255};
	static sf::Color blue4{65, 105, 255, 255};
	static sf::Color blue5{100, 149, 237, 255};
	static sf::Color red1{255, 0, 0, 255};
	static sf::Color red2{255, 69, 0, 255};
	static sf::Color red3{238, 44, 44, 255};
	static sf::Color red4{205, 85, 85, 255};
	static sf::Color red5{250, 128, 124, 255};
	static sf::Color green1{0, 255, 0, 255};
	static sf::Color green2{154, 205, 50, 255};
	static sf::Color green3{0, 255, 127, 255};
	static sf::Color green4{34, 139, 34, 255};
	static sf::Color green5{32, 178, 170, 255};
	static std::vector<sf::Color> blackandbrown{sf::Color::Black, brown1, brown2, brown3, brown4, brown5};
	static std::vector<sf::Color> red{red1, red2, red3, red4, red5};
	static std::vector<sf::Color> blue{blue1, blue2, blue3, blue4, blue5};
	static std::vector<sf::Color> green{green1, green2, green3, green4, green5};
	int count = 0;
	if (std::find(blackandbrown.begin(), blackandbrown.end(), color) != blackandbrown.end())
		count = 0;
	if (std::find(red.begin(), red.end(), color) != red.end())
		count = 3;
	if (std::find(blue.begin(), blue.end(), color) != blue.end())
		count = 2;
	if (std::find(green.begin(), green.end(), color) != green.end())
		count = 1;
	for (int i = 0; i < count;i++)
	{
		SnakeNode snakenode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x * SnakeNode::Width, nodes_[nodes_.size() - 1].getPosition().y + direction_.y * SnakeNode::Height), false, nodes_[nodes_.size() - 1].arc_);
		nodes_.push_back(snakenode);
	}
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
	SnakeNode &headNode = nodes_[0];

	for (decltype(nodes_.size()) i = 1; i < nodes_.size(); ++i)
	{
		if (distance(headNode.getPosition(), nodes_[i].getPosition()) < 3.0f)
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{

	SnakeNode &headNode = nodes_[0];
	edgecrossing(headNode);
}

void Snake::move()
{
	nodes_[0].arc_ = arc_;
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
		sf::Vector2f tmp1 = nodes_.at(i - 1).getPosition();
		sf::Vector2f tmp2 = nodes_[i].getPosition();
		sf::Vector2f tmp3 = tmp1 - tmp2;
		if (fabs(tmp3.x) > 50 || fabs(tmp3.y) > 50)
		{

			if (tmp3.x > 50)
			{
				tmp3.x = tmp1.x - Game::Width - tmp2.x;
			}
			if (tmp3.x < -50)
			{
				tmp3.x = tmp1.x + Game::Width - tmp2.x;
			}
			if (tmp3.y > 50)
			{
				tmp3.y = tmp1.y - Game::Height - tmp2.y;
			}
			if (tmp3.y < -50)
			{
				tmp3.y = tmp1.y + Game::Height - tmp2.y;
			}

			nodes_[i].setPosition(tmp2 + tmp3 * 0.1f);
			edgecrossing(nodes_[i]);
		}
		else
		{
			nodes_[i].setPosition(tmp2 + tmp3 * 0.1f);
		}
		nodes_[i].arc_ = nodes_[0].arc_;
	}
	nodes_[0].move((direction_.x * SnakeNode::Width) * 0.1, (direction_.y * SnakeNode::Height) * 0.1);
}

void Snake::render(sf::RenderWindow &window)
{
	for (auto node : nodes_)
		node.render(window);
}