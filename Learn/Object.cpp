#include "Object.h"

namespace gEng 
{

	Object::Object(sf::Vector2f position, sf::Vector2f size, sf::Color color) 
	{
		shape.setSize(size);
		shape.setPosition(position);
		shape.setFillColor(color);
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color(255, 255, 255));
	}

	void Object::setPosition(sf::Vector2f position) {
		shape.setPosition(position);
	}

	sf::Vector2f Object::getPosition() {
		return shape.getPosition();
	}

	sf::Vector2f Object::getSize() {
		return shape.getSize();
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