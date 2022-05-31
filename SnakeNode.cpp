#include <SFML/Graphics.hpp>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 10.f;
const float SnakeNode::Height = 10.f;

SnakeNode::SnakeNode(sf::Vector2f position,bool idt)
	: position_(position),ishead(idt)
{
	shape_.setPosition(position_);
	shape_.setFillColor(getacolor());
	shape_.setSize(sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
	shape_.setOutlineColor(sf::Color::White);
	shape_.setOutlineThickness(-1.f);
	
}

void SnakeNode::setPosition(sf::Vector2f position)
{
	position_ = position;
	shape_.setPosition(position_);
}

void SnakeNode::setPosition(float x, float y)
{
	position_.x = x;
	position_.y = y;
	shape_.setPosition(position_);
}

void SnakeNode::move(float xOffset, float yOffset)
{
	position_.x += xOffset;
	position_.y += yOffset;
	shape_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const
{
	return shape_.getGlobalBounds();
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}

void SnakeNode::render(sf::RenderWindow &window)
{	
	window.draw(shape_);
}
void SnakeNode::sethead()
{
	if (!ishead)
	{
		ishead = true;
	}
	return;
}
sf::Color SnakeNode::getacolor(){
	static bool target=true;
	if(ishead){
		return sf::Color::Blue;
	}
	if(target){
		target=false;
		return sf::Color::Black;
	}
	else{
		target=true;
		return sf::Color::Yellow;
	}
}