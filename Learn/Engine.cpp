#include "Engine.h"
#include <iostream>

namespace gEng
{
	//Constructor
	Engine::Engine(sf::Vector2i windowSize, sf::Vector2i terrainSize)
	{
		//create a player
		player = Player(sf::Vector2f(200, -150), sf::Vector2f(2*tileSize, 3*tileSize), sf::Color(160, 50, 104));

		//window create 
		window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Window");
		window->setFramerateLimit(60);

		//camera position
		camera.setCenter(player.getPosition().x + (player.getSize().x) / 2, player.getPosition().y + (player.getSize().y) / 2);
		camera.setSize(window->getSize().x, window->getSize().y);

		//creating terrain
		this->terrainSize = terrainSize;
		createTerrain();

	}

	//Destructor
	Engine::~Engine() {
		delete window;
	}

	//Main loop function
	void Engine::startMainLoop()
	{
		float deltaTime = 0;
		sf::Clock CurrentTime;

		//Variables related to collision
		sf::Vector2f ray_origin;
		sf::Vector2f ray_direction;
		sf::Vector2f cp;
		sf::Vector2f player_movement;

		//Main loop
		while (window->isOpen())
		{
			//Calculate delta time
			deltaTime = CurrentTime.restart().asSeconds();

			//Calculating the cell on which mouse is located
			countMouseGridPos();

			//Geting all events
			eventsControl(deltaTime, player_movement);

			//Adding gravity
			player_movement.y += 50.0f;

			//New camera position
			camera.setCenter(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getSize().y / 2);
			window->setView(camera);

			//Check player collision
			checkPlayerCollision(objVector, player, player_movement, ray_origin, ray_direction, cp, deltaTime);

			//Making the player direction vector
			sf::Vertex player_move_dir_line[] =
			{
				sf::Vertex(ray_origin),
				sf::Vertex(ray_direction)
			};

			window->clear();
			//Draw all on the screen 
			drawAll();
			window->draw(player_move_dir_line, 2, sf::Lines);
			window->display();

		}
	}

	//Function that checks all events and responds to them
	void Engine::eventsControl(float deltaTime, sf::Vector2f& player_movement) {

		//Close and resize window events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed()) {
				window->close();
			}

			if (event.type == sf::Event::Resized) {
				camera.setSize(window->getSize().x, window->getSize().y);
			}
		}

		//Button events
		//Jump
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.isOnFloor()) 
		{
			player_movement.y = -1000.0f;
		}
		//Go left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
		{
			walk_dir = -1;
		}
		//Go right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

			walk_dir = 1;
		}
		else {
			walk_dir = 0;
		}

		//Mouse events
		//Destroy block
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int block_num = terrainSize.x * mouseGridPos.y + mouseGridPos.x;

			if (block_num < terrainSize.x * terrainSize.y && objVector[block_num].getBlockType() == GRASS) {
				objVector[block_num].setColor(sf::Color(0, 0, 0));
				objVector[block_num].setBlockType(VOID);
				objVector[block_num].setColiding(false);

				player.increaseNumbersOfBlocks();
			}
		}
		//Place a block
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int block_num = terrainSize.x * mouseGridPos.y + mouseGridPos.x;

			if (block_num < terrainSize.x * terrainSize.y && 
				objVector[block_num].getBlockType() == VOID && player.getNumberOfBlocks() > 0) {

				objVector[block_num].setColor(sf::Color(128, 201, 0));
				objVector[block_num].setBlockType(GRASS);
				objVector[block_num].setColiding(true);

				player.decreaseNumbersOfBlocks();
			}
		}
	}

	//Gets the mouse position in window
	void Engine::getMouseViewPos() 
	{
		sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*window);
		mouseViewPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	}

	//Calculates the mouse position on the grid
	void Engine::countMouseGridPos() 
	{
		getMouseViewPos();
		if (mouseViewPos.x >= 0.0f) 
		{
			mouseGridPos.x = static_cast<unsigned>(mouseViewPos.x / tileSize);
		}
		if (mouseViewPos.y >= 0.0f)
		{
			mouseGridPos.y = static_cast<unsigned>(mouseViewPos.y / tileSize);
		}
	}

	//Checks the player collision
	void Engine::checkPlayerCollision(const std::vector<Object>& objVector, Player& player, sf::Vector2f& player_movement,
		                              sf::Vector2f& ray_origin, sf::Vector2f& ray_direction, sf::Vector2f& cp, float deltaTime)
	{
		std::vector<std::pair<int, float> > sortedObj;
		sf::Vector2f contact_point, contact_normal;

		bool is_up_contuct_normal = false;
		float contact_time = 0;

		//Calculate the player movement with walk direction
		player_movement = sf::Vector2f(player_movement.x + player.getSpeed() * walk_dir, player_movement.y);
		player_movement.y = std::min(player_movement.y, 1800.0f);

		//Calculates which blocks the player is colliding with and adding to sortedObj vector
		for (int i = 0; i < objVector.size(); i++)
		{
			if (objVector[i].isColiding() && DynamicRectVsRectCollision(player, objVector[i], player_movement, 
				contact_point, contact_normal, contact_time, deltaTime))
			{
				sortedObj.push_back({ i, contact_time });
				
			}
		}
		
		//Sorting the vector with colliding blocks
		std::sort(sortedObj.begin(), sortedObj.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});

		//Calculate the player new position
		for (int i = 0; i < sortedObj.size(); i++)
		{
			if (DynamicRectVsRectCollision(player, objVector[sortedObj[i].first], player_movement, contact_point,
				contact_normal, contact_time, deltaTime))
			{
				if (contact_normal == sf::Vector2f(0, -1)) {
					is_up_contuct_normal = true;
				}
				player_movement += sf::Vector2f(contact_normal.x * std::abs(player_movement.x) * (1 - contact_time), 
					                            contact_normal.y * std::abs(player_movement.y) * (1 - contact_time));
			}
		}
		sortedObj.clear();

		player.isOnFloor(is_up_contuct_normal);
		is_up_contuct_normal = false;

		//Move player to a new position
		player.move(sf::Vector2f(player_movement.x * deltaTime, player_movement.y * deltaTime));
		ray_origin = sf::Vector2f(player.getPosition().x + player.getSize().x / 2, 
			                      player.getPosition().y + player.getSize().y / 2);
		ray_direction = ray_origin + player_movement*deltaTime;
		cp = contact_point;

		if (player_movement.x != 0) {
			player_movement.x -= player.getSpeed() * walk_dir;
		}
		
	}

	//Check collision between a ray and a rect
	bool Engine::RayVsRectCollision(sf::Vector2f ray_origin, sf::Vector2f ray_direction, sf::RectangleShape target,
		                            sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near)
	{
		//Calculate the near and far collision time
		sf::Vector2f t_near = sf::Vector2f((target.getPosition().x - ray_origin.x) / ray_direction.x,
			(target.getPosition().y - ray_origin.y) / ray_direction.y);
		sf::Vector2f t_far = sf::Vector2f((target.getPosition().x + target.getSize().x - ray_origin.x) / ray_direction.x,
			(target.getPosition().y + target.getSize().y - ray_origin.y) / ray_direction.y);

		if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
		if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);


		if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

		//Calculate the actual times to the near and far collisions
		t_hit_near = std::max(t_near.x, t_near.y);
		float t_hit_far = std::min(t_far.x, t_far.y);

		if (t_hit_far < 0) return false;

		//Caluculate contact point position
		contact_point = ray_origin + t_hit_near * ray_direction;

		//Construct the normal vector
		if (t_near.x > t_near.y)
		{
			if (ray_direction.x < 0)
			{
				contact_normal = { 1, 0 };
			}
			else
			{
				contact_normal = { -1, 0 };
			}
		}
		else if (t_near.x < t_near.y)
		{
			if (ray_direction.y < 0)
			{
				contact_normal = { 0, 1 };
			}
			else
			{
				contact_normal = { 0, -1 };
			}
		}

		return true;
	}

	//Check collision between dynamic rect and static rect
	bool Engine::DynamicRectVsRectCollision(Player& in_rect, Object target, sf::Vector2f in_rect_vel,
		                                    sf::Vector2f& contact_point, sf::Vector2f& contact_normal,
		                                    float& contact_time, float deltaTime)
	{
		if (in_rect_vel.x == 0 && in_rect_vel.y == 0)
			return false;

		//Expande the static rect
		sf::RectangleShape expanded_target;
		expanded_target.setPosition(sf::Vector2f(target.getPosition().x - in_rect.getSize().x / 2, 
			                                     target.getPosition().y - in_rect.getSize().y / 2));
		expanded_target.setSize(target.getSize() + in_rect.getSize());

		//Check collision between ray and expanded rect
		if (RayVsRectCollision(sf::Vector2f(in_rect.getPosition().x + in_rect.getSize().x / 2, 
			                                in_rect.getPosition().y + in_rect.getSize().y / 2), in_rect_vel*deltaTime, 
			                                expanded_target, contact_point, contact_normal, contact_time))
		{
			return (contact_time >= 0.0f && contact_time < 1.0f);
		}

		return false;
	}

	//Normalize vector
	sf::Vector2f Engine::normalize(sf::Vector2f vector)
	{
		float lenght = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		return sf::Vector2f(vector.x / lenght, vector.y / lenght);
	}

	//Gets the grass texture
	void Engine::setGrassBlockTex(sf::Texture texture) {
		grassBlockTex = texture;
	}

	//Creating the terrain 
	void Engine::createTerrain() {
		for (int height = 0; height < terrainSize.y; height++)
		{
			for (int width = 0; width < terrainSize.x; width++)
			{
				objVector.push_back(Object(sf::Vector2f(width * tileSize, height * tileSize), 
					                       sf::Vector2f(tileSize, tileSize), sf::Color(128, 201, 0)));
			}
		}
	}

	//Drawing the terrain
	void Engine::drawTerrain()
	{
		for (int height = 0; height < terrainSize.y; height++)
		{
			for (int width = 0; width < terrainSize.x; width++)
			{
				objVector[height * terrainSize.x + width].draw(window);
			}
		}

	}

	//Drawing all 
	void Engine::drawAll() {
		drawTerrain();
		player.draw(window);
	}

}