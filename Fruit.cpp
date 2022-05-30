#include <SFML/Graphics.hpp>
#include<vector>
#include<algorithm>
#include "Fruit.h"

using namespace sfSnake;

const float Fruit::Radius = 5.f;

Fruit::Fruit(sf::Vector2f position)
{
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);

	shape_.setFillColor(getacolor());
}

void Fruit::render(sf::RenderWindow& window)
{
	window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const
{
	return shape_.getGlobalBounds();
}
sf::Color Fruit::getacolor(){
	sf::Color Gray{140,81,25,255};
	std::vector<sf::Color> Colors{sf::Color::Red,sf::Color::Black,sf::Color::Blue,sf::Color::Green,Gray};
	std::random_shuffle(Colors.begin(),Colors.end());
	return *Colors.begin();
}