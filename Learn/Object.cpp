#include "Object.h"

namespace gEng 
{

	Object::Object(sf::Vector2f position, sf::Vector2f size, sf::Color color) 
	{
		shape.setSize(size);
		shape.setPosition(position);
		shape.setFillColor(color);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(255, 255, 255));

		is_coliding = true;
	}

	void Object::setPosition(sf::Vector2f position) 
	{
		shape.setPosition(position);
	}

	void Object::setColor(sf::Color color) {
		shape.setFillColor(color);
	}

	void Object::setColiding(bool ans)
	{
		is_coliding = ans;
	}

	sf::Vector2f Object::getPosition() 
	{
		return shape.getPosition();
	}

	sf::Vector2f Object::getSize() 
	{
		return shape.getSize();
	}

	bool Object::isColiding() const {
		return is_coliding;
	}


	void Object::move(sf::Vector2f pos) 
	{
		shape.move(pos);
	}
	void Object::draw(sf::RenderWindow* window) 
	{
		window->draw(shape);
	}
}