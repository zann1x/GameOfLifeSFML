#include <iostream>
#include <cctype>

#include "application.h"
#include "config.h"

int main()
{
	Config config;

	char option;
	std::cout << "Do you want to use the default config? [Y/N]" << std::endl;
	std::cin >> option;
	option = std::toupper(option);
	switch (option)
	{
	case 'N':
		std::cout << "Nice try, loading default config anyway :)" << std::endl;
	case 'Y':
		break;
	default:
		std::cout << "Wrong input!" << std::endl;
		break;
	}
	
	std::cout << "Loading default config..." << std::endl;
	config.windowWidth = 1024;
	config.windowHeight = 768;
	config.quadSize = 8;
	config.simulationWidth = config.windowWidth / config.quadSize;
	config.simulationHeight = config.windowHeight / config.quadSize;

	std::cout << "\nCreate random world? [Y/N]" << std::endl;
	std::cin >> option;
	option = std::toupper(option);
	config.randomWorldCreation = (option == 'Y') ? true : false;

	std::cout << "Starting game..." << std::endl;
	Application app(config);
	std::cout << "running..." << std::endl;
	app.run();
	std::cout << "closing..." << std::endl;
}