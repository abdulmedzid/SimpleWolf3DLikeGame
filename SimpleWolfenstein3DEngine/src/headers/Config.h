#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <utility>

#define KeyValuePair std::pair<std::string, unsigned int>

class Config {
public:
	              Config();
private:
	KeyValuePair  splitLine(std::string line);
public:
	unsigned int  windowWidth;
	unsigned int  windowHeight;
	bool          fullscreen;
	
	unsigned int  playerFOV;
	unsigned int  rayWidth;
	unsigned int  drawDistance;

	short         redTint;
	short         greenTint;
	short         blueTint;

	bool          mapActive;
};
#endif // CONFIG_H