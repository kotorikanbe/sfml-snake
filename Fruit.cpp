#include <SFML/Graphics.hpp>
#include<vector>
#include<algorithm>
#include<random>
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
	static sf::Color brown1{140,81,25,255};
	static sf::Color brown2{139,105,20,255};
	static sf::Color brown3{139,69,19,255};
	static sf::Color brown4{165,42,42,255};
	static sf::Color brown5{205,149,12,255};
	static sf::Color blue1{152,245,255,255};
	static sf::Color blue2{0,0,255,255};
	static sf::Color blue3{135,206,250,255};
	static sf::Color blue4{65,105,255,255};
	static sf::Color blue5{100,149,237,255};
	static sf::Color red1{255,0,0,255};
	static sf::Color red2{255,69,0,255};
	static sf::Color red3{238,44,44,255};
	static sf::Color red4{205,85,85,255};
	static sf::Color red5{250,128,124,255};
	static sf::Color green1{0,255,0,255};
	static sf::Color green2{154,205,50,255};
	static sf::Color green3{0,255,127,255};
	static sf::Color green4{34,139,34,255};
	static sf::Color green5{32,178,170,255};
	static std::vector<sf::Color> Colors1{sf::Color::Black,brown1,brown2,brown3,brown4,brown5};
	static std::vector<sf::Color> Colors2{red1,red2,red3,red4,red5,blue1,blue2,blue3,blue4,blue5,green1,green2,green3,green4,green5};
	std::default_random_engine random(time(NULL));
    std::uniform_int_distribution<int> dis(0, 10000);
	int count=dis(random);
	if(count++%4==0){
		std::random_shuffle(Colors1.begin(),Colors1.end());
		return *Colors1.begin();
	}
	else{
		std::random_shuffle(Colors2.begin(),Colors2.end());
		return *Colors2.begin();
	}
}