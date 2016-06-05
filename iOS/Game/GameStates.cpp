#include <iostream>
//for debugging only

#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"
#include <thread>

/*------------------------------Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	game = game_ptr;  // pointer to state manager
	
	update_save();

	current_size = game->window.getSize();

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

	setup_text();

	background_sprite.setTexture(background); // link sprite and texture
	background_sprite.setScale(game->window.getSize().x / 1920.0f, game->window.getSize().y / 1080.0f);
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
			case sf::Event::MouseMoved:
			{
				sf::Vector2i pos = sf::Mouse::getPosition(game->window);

				if (pos.x > options[0].getPosition().x &&
					pos.x < options[0].getPosition().x + options[0].getLocalBounds().width &&
					pos.y > options[0].getPosition().y &&
					pos.y < options[3].getPosition().y + options[3].getCharacterSize())
				{
					for (selection = 0; selection < 3; selection++) {
						if (pos.y < options[selection + 1].getPosition().y) break;
					}
					selector.setPosition(options[selection].getPosition());
					selector.setPosition(selector.getPosition().x + 2, selector.getPosition().y + options[selection].getCharacterSize() + 10);
					selector.setSize(sf::Vector2f(options[selection].getLocalBounds().width, 5));

					// WORK IN PROGRESS
					/*std::thread animate_selection;
					selector.setSize(sf::Vector2f(1, 5));
					sf::Clock animator;
					while (selector.getSize().x < options[selection].getLocalBounds().width) {
						selector.setSize(sf::Vector2f(selector.getSize().x+1, 5));
						game->window.draw(selector);
						game->window.display();
						while (animator.getElapsedTime().asMicroseconds() < 100);
						animator.restart();
					}*/



					selector.setFillColor(sf::Color::White);
					std::cout << selection << std::endl;
				}

				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				sf::Vector2i pos = sf::Mouse::getPosition(game->window);

				if (pos.x > options[0].getPosition().x &&
					pos.x < options[0].getPosition().x + options[0].getLocalBounds().width &&
					pos.y > options[0].getPosition().y &&
					pos.y < selector.getPosition().y + selector.getLocalBounds().height)
				{
					std::cout << selection << std::endl;
					switch (selection) {
					case 0:
						//new game
						break;
					case 1:
						//continue game
						break;
					case 2:
						game->push_state(new options_menu(game)); //go to options
						break;
					case 3:
						game->window.close(); //quit
						break;
					default:
						break;
					}
				}
				break;
			}			
			case sf::Event::Resized: // to handle?
			{
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
		game->window.draw(options[i]);
	}
	game->window.draw(selector);
}

void main_menu::setup_text()
{
	sf::Vector2u window_size = game->window.getSize();
	sf::Vector2f starting_pos(window_size.x * 0.65f, window_size.y * 0.3f);
	int font_size = (int)(game->window.getSize().y / 13.5); // from ideal 1080p ratios
	int offset = (int)(game->window.getSize().y / 7.2); // from ideal 1080p ratios
	float old_right;

	for (int i = 0; i < 4; i++) {
		if (i == 1 && !save_game) continue;
		options[i].setFont(menu_font);
		options[i].setCharacterSize(font_size);
		options[i].setString(options_text[i]);
		options[i].setColor(sf::Color::White);
		options[i].setPosition(starting_pos);

		if (i > 0) {
			while (options[i].getGlobalBounds().left + options[i].getGlobalBounds().width < old_right) {
				options[i].setPosition(sf::Vector2f(options[i].getGlobalBounds().left + 1, options[i].getPosition().y));
			}
		}

		old_right = options[i].getGlobalBounds().left + options[i].getGlobalBounds().width;
		starting_pos.y += offset;
	}
}

void main_menu::update_save() // TO-DO
{
	save_game = true;
}







/*------------------------------Options Menu------------------------------*/


options_menu::options_menu(state_manager * game_ptr)
{
	game = game_ptr;

	/*if (!background.loadFromFile("res/png/ ----------------")) {		//to deal with
		ok = false;
		game->window.setVisible(0);
		return;
	}*/

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		ok = false;
		game->window.setVisible(false);
		return;
	}

	setup_text();

	

	//background_sprite.setTexture(background);						//to deal with
	//background_sprite.setScale(game->window.getSize().x / 1920.0f, game->window.getSize().y / 1080.0f);

}

void options_menu::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f,0.0f); // by default 

	selector.setFillColor(sf::Color::Transparent); //invisible when not in selection



	while (game->window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
			{
				if (event.key.alt && (event.key.code == sf::Keyboard::F4))
					game->window.close();
				break;
			}
			case sf::Event::MouseMoved:
			{
				mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);
				
				selection = -1; // this way the selection will always be -1 if it's not in one of the options
				for (int i  = 0; i < 5; i++)
				{
					if (options[i].getGlobalBounds().contains(mouse_pos))
						selection = i;
				}

				std::cout << "           Selection " << selection << std::endl; //debug

				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				switch (selection)
				{
					case 0:
					{
						game->pop_state();
						return;
						break;
					}
					// to add actions
				}

				break;
			}
			default:
				break;
		}
	}
}

void options_menu::logic_update(const float elapsed)
{
	if (selection != -1)
	{

		sf::Vector2f sel_pos = options[selection].findCharacterPos(0); //position of first char
		selector.setPosition(sel_pos);
		selector.move(0.0f, options[selection].getCharacterSize() +  10.0f );
		
		selector.setSize(sf::Vector2f(options[selection].getLocalBounds().width, 5));
		selector.setFillColor(sf::Color::White);

	}
}

void options_menu::draw(const float elapsed)
{
	game->window.clear(sf::Color::Black);  //temp

	//game->window.draw(background_sprite); 
	for (int i = 0; i < 5; i++)
	{
		game->window.draw(options[i]);
	}

	game->window.draw(selector);
}

void options_menu::setup_text()
{
	sf::Vector2f starting_pos(game->window.getSize().x * 0.5f, game->window.getSize().y * 0.25f);
	int font_size = (int)(game->window.getSize().y / 13.5f); // from ideal 1080p ratios
	int offset = (int)(game->window.getSize().y / 8.0f); // 


	for (int i = 0; i < 5; i++)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize(font_size);
		options[i].setString(options_str[i]);
		options[i].setColor(sf::Color::White);
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f) , (options[i].getGlobalBounds().height / 2.0f) ); // origin of font in its geometric center
		options[i].setPosition(starting_pos);

		starting_pos.y += offset;
	
	}

}
