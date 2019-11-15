#include "headers\Game.h"
#include <cmath>
using namespace std;

Game::Game(Config config)
	: windowWidth{config.windowWidth}, windowHeight{ config.windowHeight }
	, mWindow(sf::VideoMode(config.windowWidth, config.windowHeight), "RayCast")
	, mPlayer()
	, map()
	, rayWidth{config.rayWidth}
	, drawDistance{config.drawDistance}
	, FPS{30}
	, redTint{ (float)config.redTint / 255.f }
	, greenTint{ (float)config.greenTint / 255 }
	, blueTint{ (float)config.blueTint / 255 }
{
	mPlayer.FOV = config.playerFOV / 180.f * PI;
	mPlayer.position = getMapANDPlayerPosition();
}

/*****************************************************************************/
// LOADING A MAP AND RETURNING PLAYERS POSITION
sf::Vector2f Game::getMapANDPlayerPosition() {
	VectorMap vectorMap;
	// Pronalazi kartu u fajlu map.txt i vraca poziciju igraca
	sf::Vector2f playerPosition;
	std::ifstream mapData("map.txt");
	// Ako fajl postoji
	if (mapData.is_open()) {
		// najveci red
		int biggestRowLength = 0; 
		// broji vektore u glavnom vektoru
		int row = 0; 
		while (!mapData.eof()) {
			// Sve do kraja fajla
			std::string currentLine;
			std::getline(mapData, currentLine);

			if (currentLine != "") {
				// ako trenutna linija nije prazna
				// u glavni vektor dodaj novi vektor koji predstavlja trenutni red
				biggestRowLength = std::max(biggestRowLength, (int)currentLine.length());
				vectorMap.push_back(std::vector<bool>());
				for (int i = 0; i < currentLine.length(); i++) {
					if (currentLine[i] == '#') {
						vectorMap[row].push_back(true);
					}
					else if (currentLine[i] == '.') {
						vectorMap[row].push_back(false);
					}
					else if (currentLine[i] == 'P') {
						// 'P' = igrac
						playerPosition.x = row;
						playerPosition.y = i;
						vectorMap[row].push_back(false);
					}
				}
				row++;
			}
		}

		rows = row;
		columns = biggestRowLength;

		// fills blank spots
		for (auto &rowV : vectorMap) {
			int rowSize = rowV.size();
			for (int i = 0; i < biggestRowLength - rowSize; i++) {
				rowV.push_back(true);
			}
		}
		map = new bool*[row];
		for (int i = 0; i < rows; i++) {
			map[i] = new bool[biggestRowLength];
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				map[i][j] = vectorMap[i][j];
			}
		}
	}
	else {
		// save default map
		std::ofstream defMap("map.txt");
		defMap << "####################" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#............P.....#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "#..................#" << std::endl
			   << "####################" << std::endl;
		defMap.close();
		playerPosition = getMapANDPlayerPosition();
	}

	return playerPosition;
}

/*****************************************************************************/
// MAIN LOOP
void Game::run() {
	sf::Time TimePerFrame = sf::seconds(1.f / FPS);
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen()) {
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame.asSeconds());
		}
		render();
	}
}

/*****************************************************************************/
// PROCESSING EVENTS
void Game::processEvents() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::KeyPressed: processPlayerInput(event.key.code, true); break;
			case sf::Event::KeyReleased: processPlayerInput(event.key.code, false); break;
			case sf::Event::Closed: mWindow.close(); break;
		}
	}
}

/*****************************************************************************/
// PROCESSING PLAYER INPUT
void Game::processPlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::W) {
		mPlayer.movement.isMovingFowards = isPressed;
	}
	else if (key == sf::Keyboard::S) {
		mPlayer.movement.isMovingBackwards = isPressed;
	}
	else if (key == sf::Keyboard::D) {
		mPlayer.movement.isRotatingRight = isPressed;
	}
	else if (key == sf::Keyboard::A) {
		mPlayer.movement.isRotatingLeft = isPressed;
	}
}

/*****************************************************************************/
// UPDATING PLAYER POSITION AND ROTATION
void Game::update(float deltaTime) {
	float rotatingAngle = 0;
	// x i y component of a vector that represents where player looks
	float x = cos(mPlayer.angle); 
	float y = sin(mPlayer.angle);

	sf::Vector2f movement(0, 0);
	if (mPlayer.movement.isMovingFowards) {
		movement.x += x * mPlayer.movementSpeed;
		movement.y += y * mPlayer.movementSpeed;
	}
	if (mPlayer.movement.isMovingBackwards) {
		movement.x += x * mPlayer.movementSpeed * -1;
		movement.y += y * mPlayer.movementSpeed * -1;
	}
	if (mPlayer.movement.isRotatingRight) {
		rotatingAngle += mPlayer.rotatingSpeed;
	}
	if (mPlayer.movement.isRotatingLeft) {
		rotatingAngle -= mPlayer.rotatingSpeed;
	}

	if (inBounds(movement)) {
		mPlayer.move(movement);
	}
	mPlayer.rotate(rotatingAngle);
}

/*****************************************************************************/
// CHECKING IF PLAYER IN BOUNDS
bool Game::inBounds(sf::Vector2f movement) {
	float k = 1.9;

	int testX = mPlayer.position.x + movement.x * k;
	int testY = mPlayer.position.y + movement.y * k;
 	return !map[testX][testY];
}

/*****************************************************************************/
// MINIMAP
void Game::drawMap() {
	// Draws a minimap in the top left corner
	int blockWidth = 2;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (map[i][j]) {
				sf::RectangleShape block(sf::Vector2f(blockWidth, blockWidth));
				block.setFillColor(sf::Color::Red);
				block.setPosition(sf::Vector2f(i * blockWidth, j * blockWidth));
				mWindow.draw(block);
			}
		}
	}
	sf::CircleShape player(blockWidth);
	player.setPosition(sf::Vector2f(mPlayer.position.x * blockWidth, mPlayer.position.y * blockWidth));

	sf::RectangleShape eye(sf::Vector2f(1, blockWidth*3));
	sf::Vector2f eyePosition = player.getPosition();
	eyePosition.x += blockWidth;
	eyePosition.y += blockWidth;
	eye.setPosition(eyePosition);
	eye.setRotation(((float)mPlayer.angle / PI * 180 - 90));
	eye.setFillColor(sf::Color::Magenta);

	mWindow.draw(eye);
	mWindow.draw(player);
}

/*****************************************************************************/
// RAY CASTING
void Game::castRays() {
	for (unsigned int x = 0; x < windowWidth; x+=rayWidth) {
		float rayAngle = (mPlayer.angle - mPlayer.FOV / 2.f) + (float)x / windowWidth * mPlayer.FOV;
		float rayX = cos(rayAngle);
		float rayY = sin(rayAngle);
		sf::Vector2f ray(rayX, rayY);
		float distanceToWall = 0;
		bool wallHit = false;
		while (!wallHit && distanceToWall < drawDistance) {
			unsigned int testX = ray.x * distanceToWall + mPlayer.position.x;
			unsigned int testY = ray.y * distanceToWall + mPlayer.position.y;
			if (map[testX][testY]) {
				wallHit = true;
				drawWall(distanceToWall, x);
			}
			else {
				distanceToWall += 0.01;
			}
		}
	}
}
// DRAWING ONE PIXEL COLUMN
void Game::drawWall(float distanceToWall, unsigned int wallXCoordinate) {
	float ceiling = windowHeight / 2 - windowHeight / distanceToWall;
	float wallHeight = windowHeight - 2 * ceiling;

	if (wallHeight) {
		sf::RectangleShape wall(sf::Vector2f(rayWidth, wallHeight));
		wall.setPosition(wallXCoordinate, ceiling);
		//sf::Color color((int)drawDistance / distanceToWall * 10, (int)drawDistance / distanceToWall * 10, (int)drawDistance / distanceToWall * 10);
		sf::Color color(redTint * (255 - (distanceToWall / drawDistance) * 255), 
						greenTint * (255 - (distanceToWall / drawDistance) * 255),
						blueTint * (255 - (distanceToWall / drawDistance) * 255));
		wall.setFillColor(color);
		mWindow.draw(wall);
	}
}

/******************************************************************************************/
// DRAWING CEILING AND FLOOR GRADIENT
void Game::drawCeilingAndFloor() {
	sf::Color gray(redTint*180, greenTint*180, blueTint*180);
	sf::Vertex ceiling[] =
	{
		sf::Vertex(sf::Vector2f(0, 0), gray),
		sf::Vertex(sf::Vector2f(windowWidth, 0), gray),
		sf::Vertex(sf::Vector2f(windowWidth, windowHeight / 2), sf::Color::Black),
		sf::Vertex(sf::Vector2f(0, windowHeight / 2), sf::Color::Black)
	};
	sf::Vertex floor[] =
	{
		sf::Vertex(sf::Vector2f(0, windowHeight / 2), sf::Color::Black),
		sf::Vertex(sf::Vector2f(windowWidth, windowHeight / 2), sf::Color::Black),
		sf::Vertex(sf::Vector2f(windowWidth, windowHeight), gray),
		sf::Vertex(sf::Vector2f(0, windowHeight), gray),
	};
	mWindow.draw(ceiling, 4, sf::Quads);
	mWindow.draw(floor, 4, sf::Quads);
}

void Game::render() {
	mWindow.clear(sf::Color::Black);
	drawCeilingAndFloor();
	castRays();
	drawMap();
	mWindow.display();
}