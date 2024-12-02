#include "Player.h"

namespace gEng {

	Player::Player() : Object(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0)) {}
	Player::Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) : Object(position, size, color) {}

	int Player::getSpeed() {
		return speed;
	}


}