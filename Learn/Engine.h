#ifndef ENGINE_H
#define ENGINE_H

#include "Player.h"
#include <vector>



namespace gEng
{


	class Engine
	{
	private:
		sf::RenderWindow* window;
		sf::View camera;
		sf::Vector2f mouseViewPos;
		sf::Vector2i mouseGridPos;

		std::vector<Object> objVector;
		Player player;
		int walk_dir=0;

		sf::Vector2i terrainSize;
		int tileSize = 20;

		sf::Texture grassBlockTex;


	public:
		Engine(sf::Vector2i windowSize, sf::Vector2i terrainSize);
		~Engine();

		void startMainLoop();

		void eventsControl(float deltaTime, sf::Vector2f& player_move_dir);
		void getMouseViewPos();
		void countMouseGridPos();

		void checkPlayerCollision(const std::vector<Object>& objVector, Player& player, sf::Vector2f& player_movement, sf::Vector2f& ray_origin, sf::Vector2f& ray_direction, sf::Vector2f& cp, float deltaTime);
		bool RayVsRectCollision(sf::Vector2f ray_origin, sf::Vector2f ray_direction, sf::RectangleShape target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near);
		bool DynamicRectVsRectCollision(Player& in_rect, Object target, sf::Vector2f in_rect_vel, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& contact_time, float deltaTime);
		sf::Vector2f normalize(sf::Vector2f vector);

		void setGrassBlockTex(sf::Texture texture);

		void createTerrain();

		void drawTerrain();
		void drawAll();
	};

}
#endif