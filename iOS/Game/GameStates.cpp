#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"
#include "headers\Errors.h"

/*---------------- Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	this->game = game_ptr;  //now we have a pointer to state manager
	
	if (load_texture() == 42) {
		this->ok = false;
		game_ptr->window.setVisible(0);
		return;
	}

	menu_background_sprite.setTexture(background); // link sprite and texture
	//needs to add the load  of background image
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

int main_menu::load_texture()
{
	if (!background.loadFromFile("res/png/Main.png")) {
		return 42; // needs to return 42, and for error::file_access() to run on upper level
	}
	return 0;
}

void main_menu::draw(const float elapsed)
{
	game->window.draw(menu_background_sprite); // draw sprite
}
