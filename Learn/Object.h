#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>

namespace gEng 
{
	enum BLOCKTYPE { GRASS, VOID };

	class Object
	{
	protected:
		sf::RectangleShape shape;
		BLOCKTYPE block_type;
		bool is_coliding;

	public:
		//Contructor
		Object(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		//Setters
		void setPosition(sf::Vector2f position);
		void setColor(sf::Color color);
		void setBlockType(BLOCKTYPE new_block_type);
		void setColiding(bool ans);

		//Getters
		sf::Vector2f getPosition() const;
		sf::Vector2f getSize() const;
		BLOCKTYPE getBlockType() const;
		bool isColiding() const;

		//Move the object fuction
		void move(sf::Vector2f pos);

		//Draw object function
		void draw(sf::RenderWindow* window);
	};

}

#endif