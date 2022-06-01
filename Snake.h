#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>

#include "SnakeNode.h"
#include "Fruit.h"

namespace sfSnake
{
	enum class Direction
	{
		Left,
		Right,
		Up,
		Down
	};

	class Snake
	{
	public:
		Snake();

		void handleInput(sf::RenderWindow &window,std::vector<Fruit> &fruits);
		void update(sf::Time delta,std::vector<Fruit> &fruits);
		void render(sf::RenderWindow &window);

		void checkFruitCollisions(std::vector<Fruit> &fruits);

		bool hitSelf() const;

		unsigned getSize() const;
		void checkSelfCollisions();
	private:
		void move();
		void grow(sf::Color color);
		void checkEdgeCollisions();


		void initNodes();
		float arc_;
		bool hitSelf_;

		sf::Vector2f position_;
		// Direction direction_;
		sf::Vector2f direction_;

		sf::SoundBuffer pickupBuffer_;
		sf::Sound pickupSound_;

		sf::SoundBuffer dieBuffer_;
		sf::Sound dieSound_;

		std::vector<SnakeNode> nodes_;

		static const int InitialSize;
	};
}

#endif