#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

namespace gEng {

	class Player : public Object
	{
	private:
		int speed=500;

	public:
		Player();
		Player(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		sf::Vector2f getPosition();
		sf::Vector2f getSize();
		int getSpeed();
	};
}


#endif