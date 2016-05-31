#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"

/*---------------- Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	game = game_ptr;  // pointer to state manager
	
	update_save();

	if (!background.loadFromFile("res/png/Main.png")) {
		ok = false;
		game_ptr->window.setVisible(0);
		return;
	}

	if (!menu_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		ok = false;
		game_ptr->window.setVisible(0);
		return;
	}

	sf::Vector2u size_window = game->window.getSize();
	sf::Vector2f starting_pos(size_window.x * 0.65, size_window.y * 0.3);

	int old_right;
	for (int i = 0; i < 4; i++) {
		if (i == 1 && !save_game) continue;
		options_text[i].setFont(menu_font);
		options_text[i].setCharacterSize(80);
		options_text[i].setString(options[i]);
		options_text[i].setColor(sf::Color::White);
		options_text[i].setPosition(starting_pos);
		
		if (i > 0) {
			while (options_text[i].getGlobalBounds().left + options_text[i].getGlobalBounds().width < old_right) {
				options_text[i].setPosition(sf::Vector2f(options_text[i].getGlobalBounds().left + 1, options_text[i].getPosition().y));
			}
		}
		
		old_right = options_text[i].getGlobalBounds().left + options_text[i].getGlobalBounds().width;
		starting_pos.y += 150;
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
	for (int i = 0; i < 4; i++) {
		if (i == 1 && !save_game) continue;
		game->window.draw(options_text[i]);
	}
}

void main_menu::update_save() // TO-DO
{
	save_game = true;
}
