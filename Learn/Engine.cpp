#include "Engine.h"
#include <iostream>

namespace gEng
{

	Engine::Engine(sf::Vector2i windowSize, sf::Vector2i terrainSize)
	{

		player = Player(sf::Vector2f(200, -150), sf::Vector2f(100, 150), sf::Color(160, 50, 104));


		window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Window");
		window->setFramerateLimit(60);

		camera.setCenter(player.getPosition().x + (player.getSize().x) / 2, player.getPosition().y + (player.getSize().y) / 2);
		camera.setSize(window->getSize().x, window->getSize().y);

		this->terrainSize = terrainSize;
		createTerrain();


	}

	Engine::~Engine() {
		delete window;
	}

	void Engine::startMainLoop()
	{
		float deltaTime = 0;
		sf::Clock CurrentTime;

		sf::Vector2f ray_origin;
		sf::Vector2f ray_direction;
		sf::Vector2f cp;
		sf::Vector2f player_movement;

		while (window->isOpen())
		{

			deltaTime = CurrentTime.restart().asSeconds();

			countMouseGridPos();
			eventsControl(deltaTime, player_movement);
			player_movement.y += 50.0f;

			camera.setCenter(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getSize().y / 2);
			window->setView(camera);

			checkPlayerCollision(objVector, player, player_movement, ray_origin, ray_direction, cp, deltaTime);

			sf::Vertex line[] =
			{
				sf::Vertex(ray_origin),
				sf::Vertex(ray_direction)
			};

			window->clear();
			drawAll();
			window->draw(line, 2, sf::Lines);
			window->display();

		}
	}

	void Engine::eventsControl(float deltaTime, sf::Vector2f& player_movement) {

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && player.isOnFloor()) 
		{
			player_movement.y -= 1000.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
		{
			walk_dir = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

			walk_dir = 1;
		}
		else {
			walk_dir = 0;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			if (terrainSize.x * mouseGridPos.y + mouseGridPos.x <= terrainSize.x * terrainSize.y) {
				objVector[terrainSize.x * mouseGridPos.y + mouseGridPos.x].setColor(sf::Color(0, 0, 0));
			}
		}
	}

	void Engine::getMouseViewPos() 
	{
		sf::Vector2i mouseWindowPos = sf::Mouse::getPosition(*window);
		mouseViewPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	}

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
		std::cout << mouseGridPos.x << " " << mouseGridPos.y << "\t" << terrainSize.x << " " << terrainSize.y << std::endl;
	}

	void Engine::checkPlayerCollision(const std::vector<Object>& objVector, Player& player, sf::Vector2f& player_movement, sf::Vector2f& ray_origin, sf::Vector2f& ray_direction, sf::Vector2f& cp, float deltaTime)
	{
		std::vector<std::pair<int, float> > sortedObj;
		sf::Vector2f contact_point, contact_normal;

		bool is_up_contuct_normal = false;
		float contact_time = 0;

		player_movement = sf::Vector2f(player_movement.x + player.getSpeed() * walk_dir, player_movement.y);
		player_movement.y = std::min(player_movement.y, 1800.0f);

		for (int i = 0; i < objVector.size(); i++)
		{
			if (DynamicRectVsRectCollision(player, objVector[i], player_movement, contact_point, contact_normal, contact_time, deltaTime))
			{
				sortedObj.push_back({ i, contact_time });
				
			}
		}

		std::sort(sortedObj.begin(), sortedObj.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});
		for (int i = 0; i < sortedObj.size(); i++)
		{
			if (DynamicRectVsRectCollision(player, objVector[sortedObj[i].first], player_movement, contact_point, contact_normal, contact_time, deltaTime))
			{
				if (contact_normal == sf::Vector2f(0, -1)) {
					is_up_contuct_normal = true;
				}
				player_movement += sf::Vector2f(contact_normal.x * std::abs(player_movement.x) * (1 - contact_time), contact_normal.y * std::abs(player_movement.y) * (1 - contact_time));
			}
		}
		sortedObj.clear();

		player.isOnFloor(is_up_contuct_normal);
		is_up_contuct_normal = false;

		player.move(sf::Vector2f(player_movement.x * deltaTime, player_movement.y * deltaTime));
		ray_origin = sf::Vector2f(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getSize().y / 2);
		ray_direction = ray_origin + player_movement*deltaTime;
		cp = contact_point;

		if (player_movement.x != 0) {
			player_movement.x -= player.getSpeed() * walk_dir;
		}
		
	}

	bool Engine::RayVsRectCollision(sf::Vector2f ray_origin, sf::Vector2f ray_direction, sf::RectangleShape target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near)
	{
		sf::Vector2f t_near = sf::Vector2f((target.getPosition().x - ray_origin.x) / ray_direction.x,
			(target.getPosition().y - ray_origin.y) / ray_direction.y);
		sf::Vector2f t_far = sf::Vector2f((target.getPosition().x + target.getSize().x - ray_origin.x) / ray_direction.x,
			(target.getPosition().y + target.getSize().y - ray_origin.y) / ray_direction.y);

		if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
		if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;


		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

		if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

		t_hit_near = std::max(t_near.x, t_near.y);
		float t_hit_far = std::min(t_far.x, t_far.y);

		if (t_hit_far < 0) return false;

		contact_point = ray_origin + t_hit_near * ray_direction;

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

	bool Engine::DynamicRectVsRectCollision(Player& in_rect, Object target, sf::Vector2f in_rect_vel, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& contact_time, float deltaTime)
	{
		if (in_rect_vel.x == 0 && in_rect_vel.y == 0)
			return false;

		sf::RectangleShape expanded_target;
		expanded_target.setPosition(sf::Vector2f(target.getPosition().x - in_rect.getSize().x / 2, target.getPosition().y - in_rect.getSize().y / 2));
		expanded_target.setSize(target.getSize() + in_rect.getSize());

		if (RayVsRectCollision(sf::Vector2f(in_rect.getPosition().x + in_rect.getSize().x / 2, in_rect.getPosition().y + in_rect.getSize().y / 2), in_rect_vel*deltaTime, expanded_target, contact_point, contact_normal, contact_time))
		{
			return (contact_time >= 0.0f && contact_time < 1.0f);
		}

		return false;
	}

	sf::Vector2f Engine::normalize(sf::Vector2f vector)
	{
		float lenght = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		return sf::Vector2f(vector.x / lenght, vector.y / lenght);
	}

	void Engine::setGrassBlockTex(sf::Texture texture) {
		grassBlockTex = texture;
	}

	void Engine::createTerrain() {
		for (int height = 0; height < terrainSize.y; height++)
		{
			for (int width = 0; width < terrainSize.x; width++)
			{
				objVector.push_back(Object(sf::Vector2f(width * tileSize, height * tileSize), sf::Vector2f(tileSize, tileSize), sf::Color(128, 201, 0)));
			}
		}
	}

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

	void Engine::drawAll() {
		drawTerrain();
		player.draw(window);
	}

}