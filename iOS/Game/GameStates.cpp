#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"

/*---------------- Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	game = game_ptr;  // pointer to state manager
	
	if (!background.loadFromFile("res/png/Main.png")) {
		ok = false;
		game_ptr->window.setVisible(0);
		return;
	}

	background_sprite.setTexture(background); // link sprite and texture
}

void main_menu::input()
{
	sf::Event event;

	while (game->window.pollEvent(event)) {
		switch (event.type)
		{
			case sf::Event::KeyPressed:
			{
				if (event.key.alt && (event.key.code == sf::Keyboard::F4))
					game->window.close();
				break;
			}

			default:
				break;
		}


	}

	


}

void main_menu::logic_update(const float elapsed)
{
}

void main_menu::draw(const float elapsed)
{
	game->window.draw(background_sprite); // draw sprite
}
