#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"

/*---------------- Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	this->game = game_ptr;  //now we have a pointer to state manager
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

void main_menu::draw(const float elapsed)
{

	//needs background
}


