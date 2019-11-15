#include "headers/Player.h"

Movement::Movement() 
	: isMovingFowards{0}
	, isMovingBackwards{0}
	, isRotatingRight{0}
	, isRotatingLeft{0}
{}

Player::Player()
	: angle{ 0 }
	, FOV{ PI / 4.f }
	, movement()
	, movementSpeed{ 0.1f }
	, rotatingSpeed{ 0.05f }
{}

void Player::move(sf::Vector2f movement) {
	position += movement;
}

void Player::rotate(float rotatingAngle) {
	angle += rotatingAngle;
}