#include "headers/Config.h"
#include "headers/Game.h"

int main() {
	Config config;
	Game game(config);
	game.run();
}