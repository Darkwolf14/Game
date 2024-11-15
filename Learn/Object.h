#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

namespace gEng 
{

	class Object
	{
	protected:
		sf::RectangleShape shape;

	public:
		Object(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		void move(sf::Vector2f pos);
		void draw(sf::RenderWindow* window);
	};

}

#endif