#include "Player.h"
#include <iostream>

namespace gEng {

	Player::Player() : Object(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0)) 
	{
		speed = 400;
		is_on_floor = false;
	}
	Player::Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) : Object(position, size, color) {}

	bool Player::isOnFloor() 
	{
		return is_on_floor;
	}
	void Player::isOnFloor(bool res) 
	{
		is_on_floor = res;
		//std::cout << is_on_floor << std::endl;
	}

	int Player::getSpeed() {
		return speed;
	}


}