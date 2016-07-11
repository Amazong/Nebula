#include <iostream>
//for debugging only

#include "SFML\Graphics.hpp"
#include "headers\GameStates.h"
#include "headers\Errors.h"
#include <thread>

/*------------------------------Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	game = game_ptr;  // pointer to state manager
	
	update_save();

	current_size = game->window.getSize();

	if (!background.loadFromFile("res/png/Main.png")) {
		complain(ErrNo::file_access);
		return;
	}

	if (!menu_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	selection_old = -1;

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
					std::cout << selection << std::endl;
					selector.setPosition(options[selection].getPosition());
					selector.setPosition(selector.getPosition().x + 2, selector.getPosition().y + options[selection].getCharacterSize() + 10);
					//selector.setSize(sf::Vector2f(options[selection].getLocalBounds().width, 5));
					selector.setFillColor(sf::Color::White);
					
					if (selection_old != selection) {
						selector.setSize(sf::Vector2f(0, 5));
						target_size = options[selection].getLocalBounds().width;
						animation = 1;
						t_clock.restart();
					}
					
					selection_old = selection;
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
					selector.setSize(sf::Vector2f(0, 0));
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
					{
						MUSIC::get_m_player()->set_quit(true);
						
						sf::RectangleShape closer;
						closer.setSize(sf::Vector2f(game->window.getSize().x, game->window.getSize().y));
						
						sf::Clock clk;

						for (int i = 0; i < 150; i += 2) {
							clk.restart();

							closer.setFillColor(sf::Color(0, 0, 0, i));
							game->window.draw(closer);
							game->window.display();

							while (clk.getElapsedTime().asMilliseconds() < 15);
						}

						game->window.close(); //quit
						break;
					}
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
	// minimum system requirements - so long as << 333 ms, will work fine.
	// will jump in the reason of return/333
	// not serious, because smoothed with the rest

	if (animation) {
		selector.setSize(sf::Vector2f(
			(float)(target_size * (-2.0*pow(t_clock.getElapsedTime().asMilliseconds()/333.0, 3) + 3.0*pow(t_clock.getElapsedTime().asMilliseconds()/333.0, 2)))
			, (float)(selector.getSize().y)));
	}
	if (selector.getSize().x >= target_size || t_clock.getElapsedTime().asMilliseconds() >= 333) {
		animation = 0;
		t = 0;
	}
	
	/*if (animation) {
		selector.setSize(sf::Vector2f(
			target_size * (-2.0*pow(t,3) + 3.0*pow(t,2))
			, selector.getSize().y));
		t += 0.07;
	}
	if (selector.getSize().x >= target_size || t >= 1) {
		animation = 0;
		t = 0;
	}*/
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
														// will not hold exactly for all aspect ratios, but most often the difference will be negligible
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

	if (!background.loadFromFile("res/png/Options.png")) {		//to deal with
		complain(ErrNo::file_access);
		return;
	}

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}


	if (!selector_text.loadFromFile("res/png/Selector.png"))
	{
		complain(ErrNo::file_access);
		return;
	}

	selector.setTexture(selector_text);

	setup_text();

	background_sprite.setTexture(background);						//to deal with
	background_sprite.setScale(game->window.getSize().x / 1920.0f, game->window.getSize().y / 1080.0f);


	selector.setScale(0.04f, 0.04f);

	
}

void options_menu::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f,0.0f); // by default 
	


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
			
				for (int i = 0; i < 4; i++)
				{
					if (options[i].getGlobalBounds().contains(mouse_pos))
					{
						selection = i;
					}
				}

				
				std::cout << "           Selection " << selection << std::endl; //debug

				t_clock.restart();
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				switch (selection)
				{
					case 0:
						game->pop_state();
						return;
						break;
					case 1:
						if (MUSIC::get_m_player()->get_stop() == true) MUSIC::get_m_player()->set_stop(false);
						else MUSIC::get_m_player()->set_stop(true);
						break;
					case 2:
						MUSIC::get_m_player()->set_skip(true);
						break;

					default:
						break;
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
		selector.setOrigin((selector.getGlobalBounds().width / 2.0f), (selector.getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		selector.setPosition(options[selection].findCharacterPos(0).x, options[selection].getPosition().y); //position of first character
		selector.move(-20.0f , 4);
	}

}

void options_menu::draw(const float elapsed)
{
	game->window.draw(background_sprite); 

	for (int i = 0; i < 4; i++)
	{
		game->window.draw(options[i]);
	}

	if (selection != -1)
		game->window.draw(selector);
		
}

void options_menu::setup_text()
{
	sf::Vector2f starting_pos(game->window.getSize().x * 0.51f, game->window.getSize().y * 0.20f);
	int font_size = (int)(game->window.getSize().y / 13.5f); // from ideal 1080p ratios
	int offset = (int)(game->window.getSize().y / 8.0f); // from ideal 1080p ratios

	for (int i = 0; i < 4; i++)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize(font_size);
		options[i].setString(options_str[i]);
		options[i].setColor(sf::Color::White);
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f) , (options[i].getGlobalBounds().height / 2.0f) ); // origin of font in its geometric center
		options[i].setPosition(starting_pos);

		starting_pos.y += (i == 0) ? 2.5f * offset : offset;
	
	}

}


/*------------------------------ InGame ------------------------------*/

in_game::in_game(state_manager * game_ptr)
{
	game = game_ptr;
}

void in_game::update_buying_rate()
{
	buying_rate = active_store->get_max_stock() * // theoretical maximum
		active_store->get_reputation() * 2;
	if (buying_rate > 0) {
		switch (current_user->get_difficulty())
		{
		case 0:
			buying_rate *= 2.0; // can be adjusted later, according to game balance
			break;
		case 1:
			buying_rate *= 1.5; // can be adjusted later, according to game balance
			break;
		case 2:
			// rate stays as is
			break;
		default:
			// something is wrong with the user profile

			// log corrupt profile error

			complain(ErrNo::corrupt_profile);
			return;
			break;
		}
	}
}

void in_game::input()
{
	return;
}

void in_game::logic_update(const float elapsed)
{
	update_buying_rate();
	if (buying_rate > active_store->get_stock()) {
		// if not enough items in stock, penalize player
		active_store->set_reputation (active_store->get_reputation() * 0.9);
	}
}

void in_game::draw(const float elapsed)
{
	return;
}

void in_game::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();
}
