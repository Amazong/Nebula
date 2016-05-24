#include "GameWindows.h"
#include "SFML\Graphics.hpp"

int render_splash()
{
	sf::RenderWindow *splash_screen = new sf::RenderWindow(sf::VideoMode(889, 411), "", sf::Style::None);
	sf::Clock splash_clk;
	
	sf::Texture splash_png;
	if (!(splash_png.loadFromFile("res/png/Splash.png"))) return 42;
	sf::Sprite splash_sprite;
	splash_sprite.setTexture(splash_png);

	while (splash_clk.getElapsedTime().asSeconds() < 4.0f) {
		splash_screen->clear();
		splash_screen->draw(splash_sprite);
		splash_screen->display();
	}
	
	delete splash_screen;
}
