#include "Player.h"
#include <iostream>

namespace gEng {

	//Constructors
	Player::Player() : Object(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0)) 
	{
		Player::speed = 1000;
		number_of_blocks = 0;
		is_on_floor = false;
	}

	Player::Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) : Object(position, size, color) 
	{
		speed = 400;
		number_of_blocks = 0;
		is_on_floor = false;
	}

	//Setters
	//Is player on the floor?
	void Player::isOnFloor(bool res) 
	{
		is_on_floor = res;
	}

	//Increase numbers of blocks by 1
	void Player::increaseNumbersOfBlocks()
	{
		number_of_blocks++;
	}

	//Decrease numbers of blocks by 1
	void Player::decreaseNumbersOfBlocks()
	{
		number_of_blocks--;
	}

	//Setters
	bool Player::isOnFloor()
	{
		return is_on_floor;
	}

	int Player::getSpeed() {
		return speed;
	}

	int Player::getNumberOfBlocks()
	{
		return number_of_blocks;
	}

}