#include "Object.h"

namespace gEng 
{
	//Constructor
	Object::Object(sf::Vector2f position, sf::Vector2f size, sf::Color color) 
	{
		shape.setSize(size);
		shape.setPosition(position);
		shape.setFillColor(color);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(255, 255, 255));

		block_type = GRASS;
		is_coliding = true;
	}

	//Setters
	void Object::setPosition(sf::Vector2f position) 
	{
		shape.setPosition(position);
	}

	void Object::setColor(sf::Color color) 
	{
		shape.setFillColor(color);
	}

	void Object::setBlockType(BLOCKTYPE new_block_type) 
	{
		block_type = new_block_type;
	}

	void Object::setColiding(bool ans)
	{
		is_coliding = ans;
	}

	//Getters
	sf::Vector2f Object::getPosition() const
	{
		return shape.getPosition();
	}

	sf::Vector2f Object::getSize() const
	{
		return shape.getSize();
	}

	BLOCKTYPE Object::getBlockType() const
	{
		return block_type;
	}

	bool Object::isColiding() const {
		return is_coliding;
	}

	//Omve block
	void Object::move(sf::Vector2f pos) 
	{
		shape.move(pos);
	}

	//Draw block
	void Object::draw(sf::RenderWindow* window) 
	{
		window->draw(shape);
	}
}