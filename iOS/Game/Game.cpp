#include <iostream>
#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Errors.h"

int main() {
	if (render_splash() == 42) {
		error::file_access();
	}

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Working!", sf::Style::Fullscreen);

	while (window.isOpen()) {
		window.clear();
		window.display();
	}

	return(0);
}