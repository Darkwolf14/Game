#include <SFML/Graphics.hpp>
#include <iostream>

#include "Engine.h"

int main() 
{
	//Create the engine
	gEng::Engine engine(sf::Vector2i(800, 600), sf::Vector2i(20, 100));
	
	engine.startMainLoop();

	return 0;

}
