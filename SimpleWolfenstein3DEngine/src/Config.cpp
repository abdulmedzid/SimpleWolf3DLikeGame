#include "headers/Config.h"
Config::Config()
	: windowWidth{1280}
	, windowHeight{720}
	, fullscreen{false}
	, playerFOV{45}
	, rayWidth{1}
	, drawDistance{15}
	, redTint{255}, greenTint{255}, blueTint{255}
{
	std::ifstream configFile("config.txt");
	if (configFile.is_open()) {
		while (!configFile.eof()) {
			std::string currentLine;
			std::getline(configFile, currentLine);

			if (currentLine != "") {
				// par kljuc i vrijednost tj opcija i njena vrijednost
				KeyValuePair keyValue = splitLine(currentLine);
				if (keyValue.second) {
					if (keyValue.first == "windowWidth") {
						windowWidth = keyValue.second;
					}
					else if (keyValue.first == "windowHeight") {
						windowHeight = keyValue.second;
					}
					else if (keyValue.first == "fullscreen") {
						fullscreen = keyValue.second;
					}
					else if (keyValue.first == "playerFOV") {
						playerFOV = keyValue.second;
					}
					else if (keyValue.first == "rayWidth") {
						rayWidth = keyValue.second;
					}
					else if (keyValue.first == "drawDistance") {
						drawDistance = keyValue.second;
					}
					else if (keyValue.first == "redTint") {
						redTint = keyValue.second;
					}
					else if (keyValue.first == "greenTint") {
						greenTint = keyValue.second;
					}
					else if (keyValue.first == "blueTint") {
						blueTint = keyValue.second;
					}
					else if (keyValue.first == "mapActive") {
						mapActive = keyValue.second;
					}
				}
				else if (keyValue.first == "fullscreen") {
					fullscreen = 0;
				}
			}
		}
		configFile.close();
	}
	else {
		std::ofstream defFile("config.txt");
		defFile << "windowWidth:1280" << std::endl
			    << "windowHeight:720" << std::endl
			    << "fullscreen:0" << std::endl
			    << "playerFOV:45" << std::endl
			    << "rayWidth:1" << std::endl
			    << "drawDistance:15" << std::endl
			    << "redTint:255" << std::endl
			    << "greenTint:255" << std::endl
			    << "blueTint:255" << std::endl
		        << "mapActive:0" << std::endl;
		defFile.close();
	}
}

KeyValuePair Config::splitLine(std::string line) {
	// Dijeli string na 2 djela na znaku ':' i vraca par 'opciju' i 'vrijednost'
	int splitIndex = line.find(":");
	KeyValuePair keyValue;
	keyValue.first = line.substr(0, splitIndex);
	keyValue.second = stoi(line.substr(splitIndex + 1, 255));
	return keyValue;
}