#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

namespace gEng 
{

	class Object
	{
	protected:
		sf::RectangleShape shape;
		bool is_coliding;

	public:
		Object(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		void setPosition(sf::Vector2f position);
		void setColor(sf::Color color);
		void setColiding(bool ans);

		sf::Vector2f getPosition();
		sf::Vector2f getSize();
		bool isColiding() const;

		void move(sf::Vector2f pos);
		void draw(sf::RenderWindow* window);
	};

}

#endif