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
						game->push_state(new in_game(game));
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
			
				for (int i = 0; i < 3; i++)
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
		selector.setOrigin((selector.getGlobalBounds().width / 2.0f), (selector.getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		selector.setPosition(options[selection].findCharacterPos(0).x, options[selection].getPosition().y); //position of first character
		selector.move(-20.0f , 4);
	}

}

void options_menu::draw(const float elapsed)
{
	game->window.draw(background_sprite); 

	for (int i = 0; i < 3; i++)
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

	for (int i = 0; i < 3; i++)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize(font_size);
		options[i].setString(options_str[i]);
		options[i].setColor(sf::Color::White);
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f) , (options[i].getGlobalBounds().height / 2.0f) ); // origin of font in its geometric center
		options[i].setPosition(starting_pos);

		starting_pos.y += (i == 0) ? 2.5f * offset : 1.3f * offset;
		if (i == 0)
			starting_pos.y += offset* 0.2f;
	
	}

}


/*------------------------------ InGame ------------------------------*/

in_game::in_game(state_manager * game_ptr)
{
	game = game_ptr;
	
	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	setup_options();
	setup_indicators();
	setup_icons();
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
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default 
	if (selection > 2)
	{
		options[(selection - 3)].setStyle(sf::Text::Underlined);
		options[(selection - 3)].setColor(sf::Color::White);
	}



	while (game->window.pollEvent(event))
	{
		icons[5].setScale(0.2f, 0.2f);
		icons[6].setScale(0.2f, 0.2f);

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
				if (selection > 2)
				{
					options[(selection - 3)].setScale(1.0f, 1.0f);
					options[(selection - 3)].setStyle(sf::Text::Regular);
				}

				if (selection != -1)
					heat[selection].setOutlineThickness(0);

				selection = -1; // this way the selection will always be -1 if it's not in one of the options
				
				for (int i = 0; i < 4; i++)
				{
					if (heat[(3 + i)].getGlobalBounds().contains(mouse_pos))
					{
						selection = i + 3;
					}
				}

				if (selection != -1)
				{
					options[(selection - 3)].scale(1.1f, 1.1f);
					options[(selection - 3)].setStyle(sf::Text::Underlined);
				}

				control_icon_animations(mouse_pos);

				std::cout << "           Selection " << selection << std::endl; //debug

				
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (selection > 2 && event.mouseButton.button == sf::Mouse::Left)
				{
					options[(selection - 3)].setStyle(sf::Text::Italic);
					options[(selection - 3)].setColor(sf::Color::Red);	
					//options[(selection - 3)].scale(0.9f, 0.9f);
				}
				
				switch(selection)
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

void in_game::logic_update(const float elapsed)
{
	
	/*
	update_buying_rate();
	if (buying_rate > active_store->get_stock()) {
		// if not enough items in stock, penalize player
		active_store->set_reputation (active_store->get_reputation() * 0.9);
	} */
}

void in_game::draw(const float elapsed)
{
	for (int i = 0; i < 7; i++)
	{
		game->window.draw(heat[i]);
		
	}

	for (int i = 0; i < 4; i++)
		game->window.draw(options[i]);
	

	for(int i = 0 ; i < 5 ; i++)
		game->window.draw(indicators[i]);
	for (int i = 0; i < 7; i++)
		game->window.draw(icons[i]);
}		

void in_game::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();
}

void in_game::setup_options()
{
	sf::Vector2f rectangle_size(game->window.getSize().x / 5.8f , game->window.getSize().y /3.5f);
	sf::Vector2f options_pos(game->window.getSize().x / 5.8f, 0);
	
	for (int i = 0; i < 7; i++)
	{
		heat[i].setSize(rectangle_size);
		heat[i].setFillColor(sf::Color::White);
		

		switch (i)
		{
			case 0 :
			{
				heat[i].setPosition( 0 , 0 );
				heat[i].setFillColor(sf::Color::Color(35,35,35,255));
				heat[i].setOutlineColor(sf::Color::Black);
				heat[i].setOutlineThickness(-1);
				break;
			}
			case 1:
			{
				heat[i].setPosition(0, rectangle_size.y);
				heat[i].scale(1.0f, 2.0f);
				heat[i].setFillColor(sf::Color::Color(28, 28, 28, 255));
				heat[i].setOutlineColor(sf::Color::Black);
				heat[i].setOutlineThickness(-1);
				break;
			}
			case 2:
			{
				heat[i].setPosition(0, 3.0f * rectangle_size.y);
				heat[i].setFillColor(sf::Color::Color(28, 28, 28, 255));
				heat[i].scale(1.0f,0.5f);
				heat[i].setOutlineColor(sf::Color::Black);
				heat[i].setOutlineThickness(-1);
				
				break;
			}
			default:
			{	
				heat[i].scale(5.0 / 3.0, 1.0f);
				heat[i].setFillColor(sf::Color::Transparent);
				
				if (i < 5)
				{
					heat[i].setPosition(options_pos.x  + (0.55 * rectangle_size.x), 0.82 * rectangle_size.x );
					if (i == 4)
						heat[i].move( heat[i].getGlobalBounds().width + (0.35 * rectangle_size.x), 0);

				}
				else
				{
					heat[i].setPosition(heat[3].getPosition());
					heat[i].move(0, heat[i].getGlobalBounds().height + (0.225 * rectangle_size.x));
					
					if (i == 6)
						heat[i].move(heat[i].getGlobalBounds().width + (0.35 * rectangle_size.x), 0);
				}

				break;
			}

		}
	}

	
	for (int i = 0; i < 4; i++)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize((int)(game->window.getSize().y / 13.5f));
		options[i].setString(options_str[i]);
		options[i].setColor(sf::Color::White);
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f), (options[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		options[i].setPosition(heat[(3 + i)].getPosition() );
		options[i].move((heat[(3 + i)].getGlobalBounds().width / 2.0f), (heat[(3 + i)].getGlobalBounds().height / 2.3f));

	}


}

void in_game::setup_indicators()
{

	int offset = heat[1].getGlobalBounds().height / 5.0f;
	
	for (int i = 0; i < 5; i++)
	{
		indicators[i].setFont(options_font);
		indicators[i].setCharacterSize((int)(game->window.getSize().y / 22.0f));
		indicators[i].setString(indicators_str[i]);
		indicators[i].setColor(sf::Color::White);
		indicators[i].setOrigin((indicators[i].getGlobalBounds().width / 2.0f), (indicators[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		indicators[i].setPosition(heat[1].getPosition());
		indicators[i].move(heat[1].getGlobalBounds().width / 2.0f , (offset / 2.3f) + ( i*offset ) );

	}

}

void in_game::setup_icons()
{
	std::string names[7] = { "bargraph.png","staff.png","inventory.png","shop.png","finance.png","save.png", "quit.png" };

	for (int i = 0; i < 7; i++)
	{
		if (!icons_texture[i].loadFromFile("res/icons/" + names[i]))
		{
			complain(ErrNo::file_access);
			return;
		}

		icons[i].setTexture(icons_texture[i]);
		icons[i].setOrigin(icons[i].getGlobalBounds().width / 2.0f, icons[i].getGlobalBounds().height / 2.0f);
		icons[i].scale(0.5f, 0.5f);
		
		
		if (i == 2 || i == 3 )
			icons[i].scale((i == 2) ? 1.2f : 0.8f, (i == 2) ? 1.2f : 0.8f);
		

		if(i > 4)
			icons[i].setScale(0.20f, 0.20f);

		

		switch (i) // i refering to 
		{
		case 0: // bar graph
		{
			icons[i].setPosition(heat[0].getPosition().x + (heat[0].getGlobalBounds().width / 2.0f ), heat[0].getPosition().y + (heat[0].getGlobalBounds().height / 2.0f));
			break;
		}
		case 5: // save 
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width / 4.0f), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		}
		case 6: // quit game
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width * (3.0f/4.0f)), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		}

		default: // everyhting else
		{
			icons[i].setPosition(heat[(i + 2)].getPosition().x + (heat[(i + 2)].getGlobalBounds().width / 2.0f), heat[(i + 2)].getPosition().y + (heat[(i + 2)].getGlobalBounds().height / 2.0f));
			float temp = (i != 2) ? 1.5f : 2.0f;
			if (i == 3) temp = 1.3f;
			icons[i].move(0, -(icons[i].getGlobalBounds().height / temp));

			break;
		}

		}
		
	}

	

}


void in_game::control_icon_animations(sf::Vector2f mouse_pos)
{
	if (icons[5].getGlobalBounds().contains(mouse_pos))
	{
		icons[5].scale(1.1f, 1.1f);
	}
	else if (icons[6].getGlobalBounds().contains(mouse_pos))
	{
		icons[6].scale(1.1f, 1.1f);
	}
	else
	{
		icons[5].setScale(0.2f, 0.2f);
		icons[6].setScale(0.2f, 0.2f);
	}
}


