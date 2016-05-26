#include <iostream>
#include <Windows.h>

#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Errors.h"
#include "headers\StateManager.h"

int main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// Descomentar a linha acima para esconder a consola
	

	if (render_splash() == 42) {
		error::file_access();
	}

	
	state_manager game;

	game.game_loop();

	/*while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
					window.clear(sf::Color::Blue);
					window.display();
					sf::Clock wait2;
					while (wait2.getElapsedTime().asSeconds() < 2.0f);
					window.close();
				}
			}
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::F4)) {
			window.clear(sf::Color::Blue);
			window.display();
			sf::Clock wait2;
			while (wait2.getElapsedTime().asSeconds() < 2.0f);
			window.close();
		}

		window.clear();
		window.display();
	}*/

	return(0);
}