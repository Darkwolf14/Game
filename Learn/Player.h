#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

namespace gEng {

	class Player : public Object
	{
	private:
		int speed;
		int number_of_blocks;
		bool is_on_floor;

	public:

		//Constructors
		Player();
		Player(sf::Vector2f position, sf::Vector2f size, sf::Color color);

		//Setters
		void isOnFloor(bool res);
		void increaseNumbersOfBlocks();
		void decreaseNumbersOfBlocks();

		//Getters
		bool isOnFloor();
		int getSpeed();
		int getNumberOfBlocks();
	};
}


#endif