#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System/Vector2.hpp>

#define PI 3.1415f

struct Movement {
	Movement();
	bool isMovingFowards;
	bool isMovingBackwards;
	bool isRotatingRight;
	bool isRotatingLeft;
};

class Player {
public:
				    Player();
	void		    move(sf::Vector2f movement);
	void            rotate(float rotatingAngle);
public:
	sf::Vector2f    position;
	float			angle;
	float           FOV;
	float           rotatingSpeed;
	float           movementSpeed;
	Movement        movement;
};

#endif // PLAYER_H