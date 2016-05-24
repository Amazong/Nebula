#include "SFML\Graphics.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(1618, 1000), "Working!");

	while (window.isOpen()) {
		window.clear();
		window.display();
	}

	return(0);
}