#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

namespace gEng {

	class Player : public Object
	{
	private:
		int speed=400;

	public:
		Player();
		Player(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		
		int getSpeed();
	};
}


#endif