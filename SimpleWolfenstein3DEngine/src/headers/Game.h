#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Config.h"

#define VectorMap std::vector<std::vector<bool>> 

class Game {
public:
			 	      Game(Config config);
	void	 	      run();

private:
	sf::Vector2f      getMapANDPlayerPosition(); // returns player position
	void              processEvents();
	void              update(float deltaTime);
	void              processPlayerInput(sf::Keyboard::Key key, bool isPressed);
	bool              inBounds(sf::Vector2f movement); // checks if player in bounds
	void              render();
	void              drawCeilingAndFloor();
	void              castRays();
	void              drawWall(float distanceToWall, unsigned int wallXCoordinate);
	void              drawMap();

private:
	sf::RenderWindow  mWindow;
	unsigned int      windowWidth;
	unsigned int      windowHeight;
	unsigned int      FPS;
	
	unsigned int      rayWidth;
	unsigned int      drawDistance;
	float             redTint;
	float             greenTint;
	float             blueTint;

	bool**            map;
	unsigned int      rows;
	unsigned int      columns;

	Player            mPlayer;
};
#endif // GAME_H