#include <SFML/Graphics.hpp>

#include "SnakeNode.h"
#include <random>
using namespace sfSnake;



SnakeNode::SnakeNode(sf::Vector2f position, bool idt, float arc)
	: position_(position), ishead_(idt), arc_(arc)
{
	if(ishead_){
		texture.loadFromFile("Pictures/head.png");
	}
	else{
		texture.loadFromFile("Pictures/body.png");
	}
	sprite.setTexture(texture);
	sprite.setPosition(position_);
	sprite.setScale(sf::Vector2f(0.1, 0.1));
}

void SnakeNode::setPosition(sf::Vector2f position)
{

		position_ = position;
		sprite.setPosition(position_);
	
}

void SnakeNode::setPosition(float x, float y)
{
	position_.x = x;
	position_.y = y;

	sprite.setPosition(position_);
}

void SnakeNode::move(float xOffset, float yOffset)
{
	position_.x += xOffset;
	position_.y += yOffset;

	sprite.setPosition(position_);

}

sf::FloatRect SnakeNode::getBounds() const
{
	
	return sprite.getGlobalBounds();
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}

void SnakeNode::render(sf::RenderWindow &window)
{

		sprite.setTexture(texture);
		sprite.setPosition(position_);
		sprite.setRotation(arc_ + 90);
		window.draw(sprite);
	
	
}
void SnakeNode::sethead()
{
	if (!ishead_)
	{
		ishead_ = true;
	}
	return;
}
static SnakeNode snakenode;
const float SnakeNode::Width = snakenode.getBounds().width;
const float SnakeNode::Height = snakenode.getBounds().height;
// sf::Color SnakeNode::getacolor()
// {
// 	static bool target = true;
// 	static sf::Color brown1{140, 81, 25, 255};
// 	static sf::Color brown2{139, 105, 20, 255};
// 	static sf::Color brown3{139, 69, 19, 255};
// 	static sf::Color brown4{165, 42, 42, 255};
// 	static sf::Color brown5{205, 149, 12, 255};
// 	static sf::Color blue1{152, 245, 255, 255};
// 	static sf::Color blue2{0, 0, 255, 255};
// 	static sf::Color blue3{135, 206, 250, 255};
// 	static sf::Color blue4{65, 105, 255, 255};
// 	static sf::Color blue5{100, 149, 237, 255};
// 	static sf::Color red1{255, 0, 0, 255};
// 	static sf::Color red2{255, 69, 0, 255};
// 	static sf::Color red3{238, 44, 44, 255};
// 	static sf::Color red4{205, 85, 85, 255};
// 	static sf::Color red5{250, 128, 124, 255};
// 	static sf::Color green1{0, 255, 0, 255};
// 	static sf::Color green2{154, 205, 50, 255};
// 	static sf::Color green3{0, 255, 127, 255};
// 	static sf::Color green4{34, 139, 34, 255};
// 	static sf::Color green5{32, 178, 170, 255};
// 	static std::vector<sf::Color> Colors1{sf::Color::Black, brown1, brown2, brown3, brown4, brown5};
// 	static std::vector<sf::Color> Colors2{red1, red2, red3, red4, red5, blue1, blue2, blue3, blue4, blue5, green1, green2, green3, green4, green5};
// 	if (target)
// 	{
// 		target = false;
// 		std::random_shuffle(Colors1.begin(),Colors1.end());
// 		return *Colors1.begin();
// 	}
// 	else
// 	{
// 		target = true;
// 		std::random_shuffle(Colors2.begin(),Colors2.end());
// 		return *Colors2.begin();
// 	}
// }