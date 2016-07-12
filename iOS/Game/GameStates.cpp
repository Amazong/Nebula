#include "headers\GameStates.h"


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
						game->push_state(new new_game1(game, game->window.capture()));
						break;
					case 1:
						game->push_state(new continue_game(game, game->window.capture()));
						break;
					case 2:
						game->push_state(new options_menu(game)); //go to options
						break;
					case 3:
					{
						MUSIC::get_m_player()->set_quit(true);
						
						sf::RectangleShape closer;
						float size_x, size_y;
						size_x = (float)game->window.getSize().x;
						size_y = (float)game->window.getSize().y;
						closer.setSize(sf::Vector2f(size_x, size_y));
						
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
			
				for (int i = 0; i < 3; i++)
				{
					if (options[i].getGlobalBounds().contains(mouse_pos))
					{
						selection = i;
					}
				}

				
				std::cout << "           Selection " << selection << std::endl; //debug

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
			if (handle_icons((sf::Vector2f) sf::Mouse::getPosition(game->window)))
				return;

			if (selection > 2 && event.mouseButton.button == sf::Mouse::Left)
			{
				options[(selection - 3)].setStyle(sf::Text::Italic);
				options[(selection - 3)].setColor(sf::Color::Red);
				//options[(selection - 3)].scale(0.9f, 0.9f);
			}

			switch (selection)
			{
			case 0:
				game->pop_state();
				return;
				break;
			case 4:
				game->push_state(new inventory(game));
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


	for (int i = 0; i < 5; i++)
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
	sf::Vector2f rectangle_size(game->window.getSize().x / 5.8f, game->window.getSize().y / 3.5f);
	sf::Vector2f options_pos(game->window.getSize().x / 5.8f, 0);

	for (int i = 0; i < 7; i++)
	{
		heat[i].setSize(rectangle_size);
		heat[i].setFillColor(sf::Color::White);


		switch (i)
		{
		case 0:
		{
			heat[i].setPosition(0, 0);
			heat[i].setFillColor(sf::Color::Color(53, 53, 53, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		}
		case 1:
		{
			heat[i].setPosition(0, rectangle_size.y);
			heat[i].scale(1.0f, 2.0f);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		}
		case 2:
		{
			heat[i].setPosition(0, 3.0f * rectangle_size.y);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].scale(1.0f, 0.5f);
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);

			break;
		}
		default:
		{
			heat[i].scale(5.0f / 3.0f, 1.0f);
			heat[i].setFillColor(sf::Color::Transparent);

			if (i < 5)
			{
				heat[i].setPosition(options_pos.x + (0.55f * rectangle_size.x), 0.82f * rectangle_size.x);
				if (i == 4)
					heat[i].move(heat[i].getGlobalBounds().width + (0.35f * rectangle_size.x), 0.0f);

			}
			else
			{
				heat[i].setPosition(heat[3].getPosition());
				heat[i].move(0.0f, heat[i].getGlobalBounds().height + (0.225f * rectangle_size.x));

				if (i == 6)
					heat[i].move(heat[i].getGlobalBounds().width + (0.35f * rectangle_size.x), 0.0f);
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
		options[i].setPosition(heat[(3 + i)].getPosition());
		options[i].move((heat[(3 + i)].getGlobalBounds().width / 2.0f), (heat[(3 + i)].getGlobalBounds().height / 2.3f));
	}
}

void in_game::setup_indicators()
{

	int offset = (int)(heat[1].getGlobalBounds().height / 5.0f);

	for (int i = 0; i < 5; i++)
	{
		indicators[i].setFont(options_font);
		indicators[i].setCharacterSize((int)(game->window.getSize().y / 22.0f));
		indicators[i].setString(indicators_str[i]);
		indicators[i].setColor(sf::Color::White);
		indicators[i].setOrigin((indicators[i].getGlobalBounds().width / 2.0f), (indicators[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		indicators[i].setPosition(heat[1].getPosition());
		indicators[i].move(heat[1].getGlobalBounds().width / 2.0f, (offset / 2.3f) + (i*offset));

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


		if (i == 2 || i == 3)
			icons[i].scale((i == 2) ? 1.2f : 0.8f, (i == 2) ? 1.2f : 0.8f);


		if (i > 4)
			icons[i].setScale(0.20f, 0.20f);



		switch (i) // i refering to 
		{
		case 0: // bar graph
		{
			icons[i].setPosition(heat[0].getPosition().x + (heat[0].getGlobalBounds().width / 2.0f), heat[0].getPosition().y + (heat[0].getGlobalBounds().height / 2.0f));
			break;
		}
		case 5: // save 
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width / 4.0f), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		}
		case 6: // quit game
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width * (3.0f / 4.0f)), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
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

bool in_game::handle_icons(sf::Vector2f mouse_pos)
{
	if (icons[6].getGlobalBounds().contains(mouse_pos))
	{
		game->pop_state();
		return(true);
	}


	if (icons[5].getGlobalBounds().contains(mouse_pos))
	{
		std::string a("hello");
		show_textbox(a, 20, 12);
		return(true);
	}
	return(false);
}


/*--------------------------- InGameSetup ------------------------------*/

in_game_setup::in_game_setup(state_manager * game_ptr)
{
	game = game_ptr;

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	setup_options();
	setup_indicators();
	setup_icons();

	start_text.setFont(options_font);
	start_text.setString("Start!");
	start_text.setColor(sf::Color::White);
	start_text.setCharacterSize(80);
	start_text.setPosition(game->window.getSize().x - start_text.getGlobalBounds().width - 80, 20);
}

void in_game_setup::update_buying_rate()
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

void in_game_setup::input()
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
			if (handle_icons((sf::Vector2f) sf::Mouse::getPosition(game->window)))
				return;

			if (selection > 2 && event.mouseButton.button == sf::Mouse::Left)
			{
				options[(selection - 3)].setStyle(sf::Text::Italic);
				options[(selection - 3)].setColor(sf::Color::Red);
				//options[(selection - 3)].scale(0.9f, 0.9f);
			}

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

void in_game_setup::logic_update(const float elapsed)
{

	/*
	update_buying_rate();
	if (buying_rate > active_store->get_stock()) {
	// if not enough items in stock, penalize player
	active_store->set_reputation (active_store->get_reputation() * 0.9);
	} */
}

void in_game_setup::draw(const float elapsed)
{
	for (int i = 0; i < 7; i++)
		game->window.draw(heat[i]);

	for (int i = 0; i < 4; i++)
		game->window.draw(options[i]);

	for (int i = 0; i < 5; i++)
		game->window.draw(indicators[i]);

	for (int i = 0; i < 7; i++)
		game->window.draw(icons[i]);

	game->window.draw(start_text);
}

void in_game_setup::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();
}

void in_game_setup::setup_options()
{
	sf::Vector2f rectangle_size(game->window.getSize().x / 5.8f, game->window.getSize().y / 3.5f);
	sf::Vector2f options_pos(game->window.getSize().x / 5.8f, 0);

	for (int i = 0; i < 7; i++)
	{
		heat[i].setSize(rectangle_size);
		heat[i].setFillColor(sf::Color::White);


		switch (i)
		{
		case 0:
		{
			heat[i].setPosition(0, 0);
			heat[i].setFillColor(sf::Color::Color(53, 53, 53, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		}
		case 1:
		{
			heat[i].setPosition(0, rectangle_size.y);
			heat[i].scale(1.0f, 2.0f);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		}
		case 2:
		{
			heat[i].setPosition(0, 3.0f * rectangle_size.y);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].scale(1.0f, 0.5f);
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);

			break;
		}
		default:
		{
			heat[i].scale(5.0f / 3.0f, 1.0f);
			heat[i].setFillColor(sf::Color::Transparent);

			if (i < 5)
			{
				heat[i].setPosition(options_pos.x + (0.55f * rectangle_size.x), 0.82f * rectangle_size.x);
				if (i == 4)
					heat[i].move(heat[i].getGlobalBounds().width + (0.35f * rectangle_size.x), 0.0f);

			}
			else
			{
				heat[i].setPosition(heat[3].getPosition());
				heat[i].move(0.0f, heat[i].getGlobalBounds().height + (0.225f * rectangle_size.x));

				if (i == 6)
					heat[i].move(heat[i].getGlobalBounds().width + (0.35f * rectangle_size.x), 0.0f);
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
		options[i].setPosition(heat[(3 + i)].getPosition());
		options[i].move((heat[(3 + i)].getGlobalBounds().width / 2.0f), (heat[(3 + i)].getGlobalBounds().height / 2.3f));
	}
}

void in_game_setup::setup_indicators()
{

	int offset = (int)(heat[1].getGlobalBounds().height / 5.0f);

	for (int i = 0; i < 5; i++)
	{
		indicators[i].setFont(options_font);
		indicators[i].setCharacterSize((int)(game->window.getSize().y / 22.0f));
		indicators[i].setString(indicators_str[i]);
		indicators[i].setColor(sf::Color::White);
		indicators[i].setOrigin((indicators[i].getGlobalBounds().width / 2.0f), (indicators[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		indicators[i].setPosition(heat[1].getPosition());
		indicators[i].move(heat[1].getGlobalBounds().width / 2.0f, (offset / 2.3f) + (i*offset));

	}

}

void in_game_setup::setup_icons()
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


		if (i == 2 || i == 3)
			icons[i].scale((i == 2) ? 1.2f : 0.8f, (i == 2) ? 1.2f : 0.8f);


		if (i > 4)
			icons[i].setScale(0.20f, 0.20f);



		switch (i) // i refering to 
		{
		case 0: // bar graph
		{
			icons[i].setPosition(heat[0].getPosition().x + (heat[0].getGlobalBounds().width / 2.0f), heat[0].getPosition().y + (heat[0].getGlobalBounds().height / 2.0f));
			break;
		}
		case 5: // save 
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width / 4.0f), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		}
		case 6: // quit game
		{
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width * (3.0f / 4.0f)), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
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

void in_game_setup::control_icon_animations(sf::Vector2f mouse_pos)
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

bool in_game_setup::handle_icons(sf::Vector2f mouse_pos)
{
	if (icons[6].getGlobalBounds().contains(mouse_pos))
	{
		game->pop_state();
		return(true);
	}


	if (icons[5].getGlobalBounds().contains(mouse_pos))
	{
		std::string a("hello");
		show_textbox(a, 20, 12);
		return(true);
	}
	return(false);
}


/*------------------------------ New_Game1 ------------------------------*/

new_game1::new_game1(state_manager * game, sf::Image Background_img)
{
	this->game = game;

	if (!background.loadFromImage(Background_img))
	{
		complain(ErrNo::capture_screen_error);
		return;
	}

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	background_sprite.setTexture(background);
	

	input_place.setFillColor(sf::Color::Color(0, 0, 0, 235));
	input_place.setSize(sf::Vector2f((8.0f/16.0f) * game->window.getSize().x , (game->window.getSize().y * (2.0f/3.0f))));
	input_place.setPosition(game->window.getSize().x * (7.0f / 16.0f), game->window.getSize().y / 5.5f);
	input_place.setOutlineColor(sf::Color(72, 72, 72, 255));
	input_place.setOutlineThickness(-3);

	text_enter.setSize(sf::Vector2f(input_place.getGlobalBounds().width / 2.0f, input_place.getGlobalBounds().height / 6.0f));
	text_enter.setOrigin(text_enter.getGlobalBounds().width / 2.0f, text_enter.getGlobalBounds().height / 2.0f);
	text_enter.setFillColor(sf::Color::Color(72, 72, 72, 255));
	text_enter.setPosition(input_place.getPosition().x + (input_place.getGlobalBounds().width / 2.0f), input_place.getPosition().y + (input_place.getGlobalBounds().height / 4.0f));

	setup_options();
}

void new_game1::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default 




	while (game->window.pollEvent(event))
	{
		for (int i = 2; i < 6; i++)
		{
			options[i].setStyle(sf::Text::Regular);
		}
		options[(difficulty + 2)].setColor(sf::Color::White);
		
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

			for (int i = 2; i < 6; i++)
			{
				if (options[i].getGlobalBounds().contains(mouse_pos))
				{
					selection = i;
				}
			}
			
			text_enter.setScale(1.0f, 1.0f);

			if (text_enter.getGlobalBounds().contains(mouse_pos))
			{
				selection = 9;
				text_enter.scale(1.1f, 1.1f);
			}

			if (selection != 9)
				text_enter.setFillColor(sf::Color(72, 72, 72, 255));
			
			
			if (selection != -1 && selection != 9 && selection != (2 + difficulty))
			{
				options[selection].setStyle(sf::Text::Underlined);
			}
			

			

			std::cout << "           Selection " << selection << std::endl; //debug


			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
			if (!input_place.getGlobalBounds().contains(mouse_pos))
			{
				game->pop_state();
				return;
			}

			if (selection == -1) break;
			switch (selection)
			{
				case 5:
				{
					game->get_current_user()->set_difficulty(difficulty);
					switch (difficulty) {
					case 0:
						game->get_current_user()->set_net_worth(400000);
						break;
					case 1:
						game->get_current_user()->set_net_worth(300000);
						break;
					case 2:
						game->get_current_user()->set_net_worth(200000);
						break;
					default:
						break;
					}
					game->get_current_user()->set_user_name(name);
					game->change_state(new in_game_setup(game));
					return;
					break;
				}
				case 9:
				{
					text_enter.setFillColor(sf::Color::White);
					break;
				}
				default:
				{
					options[difficulty + 2].setColor(sf::Color(72, 72, 72, 255));
					difficulty = selection - 2;
					break;
				}
			
			}

			break;
		}
		case sf::Event::TextEntered:
		{
			if (text_enter.getFillColor() == sf::Color::White)
			{
				if (name.getSize() > 50 || name_text.getGlobalBounds().width >= (text_enter.getGlobalBounds().width - 30) )
				{
					if (event.text.unicode == 8)
					{
						if (name.isEmpty()) break;
						name.erase(name.getSize() - 1);
						name_text.setString(name);
						name_text.setOrigin(name_text.getGlobalBounds().width / 2.0f, name_text.getGlobalBounds().height / 2.0f);
						name_text.setPosition(text_enter.getPosition());
						break;
					}
					break;
				}
				if (event.text.unicode < 128)
				{
					if (event.text.unicode == 8)
					{
						if (name.isEmpty()) break;
						name.erase(name.getSize() - 1);
						name_text.setString(name);
						name_text.setOrigin(name_text.getGlobalBounds().width / 2.0f, name_text.getGlobalBounds().height / 2.0f);
						name_text.setPosition(text_enter.getPosition());
						break;
					}

					name += static_cast<char>(event.text.unicode);
					name_text.setString(name);
					name_text.setOrigin(name_text.getGlobalBounds().width / 2.0f, name_text.getGlobalBounds().height / 2.0f);
					name_text.setPosition(text_enter.getPosition());
				}
			}
			break;
		}
		default:
			break;
		}
	}
}

void new_game1::logic_update(const float elapsed)
{
}

void new_game1::draw(const float elapsed)
{
	game->window.draw(background_sprite);
	game->window.draw(input_place);
	game->window.draw(text_enter);

	for (int i = 0; i < 6; i++)
	{
		game->window.draw(options[i]);
	}

	game->window.draw(name_text);
}

void new_game1::setup_options()
{
	
	for (int i = 0; i < 6; i++)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize((int)(game->window.getSize().y / 13.5f));
		options[i].setString(options_str[i]);
		options[i].setColor(sf::Color(72, 72, 72, 255));
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f), (options[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		options[i].setPosition(text_enter.getPosition());
		switch (i)
		{
			case 0:
			{
				options[i].move(0, -(input_place.getGlobalBounds().height / 5.0f));
				break;
			}
			case 1:
			{
				options[i].move(0, input_place.getGlobalBounds().height / 5.0f);
				break;
			}
			case 5:
			{
				options[i].move(0, (input_place.getGlobalBounds().height  * 3.0f )/ 5.0f);
				break;
			}
			default:
			{
				options[i].move(0, (input_place.getGlobalBounds().height  * 2.0f) / 5.0f);
				break;
			}
		}
		
	}

	options[2].move(-(input_place.getGlobalBounds().width / 3.0f), 0);
	options[4].move((input_place.getGlobalBounds().width / 3.0f), 0);

	name_text.setFont(options_font);
	name_text.setColor(sf::Color::Black);


}

/*------------------------------ Continue ------------------------------*/

continue_game::continue_game(state_manager * game_ptr, sf::Image background)
{
	game = game_ptr;
	background_texture.loadFromImage(background);
	background_sprite.setTexture(background_texture);

	if (!font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	continue_background.setFillColor(sf::Color::Color(0, 0, 0, 235));
	continue_background.setSize(sf::Vector2f((8.0f / 16.0f) * game->window.getSize().x, (game->window.getSize().y * (2.0f / 3.0f))));
	continue_background.setPosition(game->window.getSize().x * (7.0f / 16.0f), game->window.getSize().y / 5.5f);
	continue_background.setOutlineColor(sf::Color(72, 72, 72, 255));
	continue_background.setOutlineThickness(-3);

	setup_text();
}

void continue_game::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default 

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
			for (int i = 0; i < n_saves; i++) {
				saved_profiles[i].setColor(sf::Color(190, 190, 190, 255));
			}

			for (int i = 0; i < 3; i++)
			{
				if (saved_profiles[i].getGlobalBounds().contains(mouse_pos))
				{
					selection = i;
					saved_profiles[i].setColor(sf::Color::White);
					for (int j = 0; j < n_saves; j++) {
						if (j == i) continue;
						saved_profiles[j].setColor(sf::Color(190, 190, 190, 255));
					}
				}
			}

			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (selection != -1) {
				if (game->get_current_user() != nullptr) {
					delete game->get_current_user();
				}

				std::string user = saved_profiles[selection].getString();
				user.erase(0, 3);

				game->set_current_user(new user_profile());

				game->get_current_user()->load_user(user);

				game->set_current_user(new user_profile());

				game->get_current_user()->load_user(user);

				game->change_state(new in_game(game));

				return;
			}

			game->pop_state();
			return;
			break;
		}
		default:
			break;
		}
	}
}

void continue_game::logic_update(const float elapsed)
{
}

void continue_game::draw(const float elapsed)
{
	game->window.clear();
	game->window.draw(background_sprite);
	game->window.draw(continue_background);
	game->window.draw(title);
	for (int i = 0; i < n_saves; i++) {
		game->window.draw(saved_profiles[i]);
	}
}

void continue_game::setup_text()
{
	std::ifstream loader("saves/users");
	std::string cur_prof;

	n_saves = 0;

	if (!loader.is_open()) {
		return;
	}

	while (!loader.eof()) {
		std::getline(loader, cur_prof);
		if (cur_prof != "") {
			saved_profiles[n_saves].setString(std::to_string(n_saves + 1) + ". " + cur_prof);
			n_saves++;
		}
	}

	title.setFont(font);
	title.setString("Choose a profile:");
	title.setCharacterSize(60);
	title.setColor(sf::Color::White);
	title.setPosition(continue_background.getPosition());

	sf::FloatRect textRect = title.getLocalBounds();

	title.move(sf::Vector2f(((continue_background.getLocalBounds().width - textRect.width) / 2.0f), 25));

	for (int i = 0; i < n_saves; i++) {
		saved_profiles[i].setFont(font);
		saved_profiles[i].setCharacterSize(50);
		saved_profiles[i].setColor(sf::Color(190, 190, 190, 255));
		saved_profiles[i].setPosition(continue_background.getPosition());

		saved_profiles[i].move(sf::Vector2f(70, i * 80 + (title.getLocalBounds().height + 100)));
	}
}


/*------------------------------ Inventory ------------------------------*/

inventory::inventory(state_manager * game_ptr)
{
	game = game_ptr;

	if (!font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	details.setFillColor(sf::Color::Color(170, 170, 170, 235));
	details.setSize(sf::Vector2f((8.0f / 16.0f) * game->window.getSize().x, (game->window.getSize().y * (2.0f / 3.0f))));
	details.setPosition(game->window.getSize().x * (7.0f / 16.0f), game->window.getSize().y / 5.5f);
	details.setOutlineColor(sf::Color(100, 100, 100, 255));
	details.setOutlineThickness(-3);

	setup();
}

void inventory::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default 
	
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
				
			}
			
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (selection > 2 && event.mouseButton.button == sf::Mouse::Left)
			{
			
			}

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

void inventory::logic_update(const float elapsed)
{

}

void inventory::draw(const float elapsed)
{
	game->window.draw(details);
	game->window.draw(buy);
	game->window.draw(back);

	for (int i = 0; i < 3; i++)
		game->window.draw(heat[i]);
		
	for (int i = 0; i < 5; i++)
		game->window.draw(indicators[i]);

	for (int i = 0; i < 3; i++)
		game->window.draw(icons[i]);

	for (int i = 0; i < 5; i++)
		game->window.draw(currently_showing[i]);
}

void inventory::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();

	setup_options();
	setup_text();
	setup_icons();
}

void inventory::setup_options()
{
	sf::Vector2f rectangle_size(game->window.getSize().x / 5.8f, game->window.getSize().y / 3.5f);
	sf::Vector2f options_pos(game->window.getSize().x / 5.8f, 0);

	for (int i = 0; i < 3; i++)
	{
		heat[i].setSize(rectangle_size);
		heat[i].setFillColor(sf::Color::White);

		switch (i)
		{
		case 0:
			heat[i].setPosition(0, 0);
			heat[i].setFillColor(sf::Color::Color(53, 53, 53, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		case 1:
			heat[i].setPosition(0, rectangle_size.y);
			heat[i].scale(1.0f, 2.0f);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		case 2:
			heat[i].setPosition(0, 3.0f * rectangle_size.y);
			heat[i].setFillColor(sf::Color::Color(40, 40, 40, 255));
			heat[i].scale(1.0f, 0.5f);
			heat[i].setOutlineColor(sf::Color::Black);
			heat[i].setOutlineThickness(-1);
			break;
		}
	}
}

void inventory::setup_text()
{
	int offset = (int)(heat[1].getGlobalBounds().height / 5.0f);

	for (int i = 0; i < 5; i++)	{
		indicators[i].setFont(font);
		indicators[i].setCharacterSize((int)(game->window.getSize().y / 22.0f));
		indicators[i].setString(indicators_str[i]);
		indicators[i].setColor(sf::Color::White);
		indicators[i].setOrigin((indicators[i].getGlobalBounds().width / 2.0f), (indicators[i].getGlobalBounds().height / 2.0f)); // origin of font in its geometric center
		indicators[i].setPosition(heat[1].getPosition());
		indicators[i].move(heat[1].getGlobalBounds().width / 2.0f, (offset / 2.3f) + (i*offset));
	}

	for (int i = 0; i < 5; i++) {
		currently_showing[i].setFont(font);
		currently_showing[i].setCharacterSize((int)(game->window.getSize().y / 16.0f));
		currently_showing[i].setColor(sf::Color::White);
		currently_showing[i].setPosition(heat[1].getGlobalBounds().width + 90, (2 * i * currently_showing[i].getCharacterSize() + game->window.getSize().y / 5.0f));
	}
	update_list();

	buy.setFont(font);
	buy.setColor(sf::Color::White);
	buy.setCharacterSize(50);
	buy.setString("Buy Inventory");
	buy.setPosition((game->window.getSize().x - buy.getGlobalBounds().width - 50),
		game->window.getSize().y - (float)2 * buy.getCharacterSize());

	back.setFont(font);
	back.setColor(sf::Color::White);
	back.setCharacterSize(50);
	back.setString("Back");
	back.setPosition(heat[0].getGlobalBounds().width + 50,
		game->window.getSize().y - (float)2 * buy.getCharacterSize());
}

void inventory::setup_icons()
{
	std::string names[3] = { "bargraph.png", "save.png", "quit.png" };

	for (int i = 0; i < 3; i++)
	{
		if (!icons_texture[i].loadFromFile("res/icons/" + names[i]))
		{
			complain(ErrNo::file_access);
			return;
		}

		icons[i].setTexture(icons_texture[i]);
		icons[i].setOrigin(icons[i].getGlobalBounds().width / 2.0f, icons[i].getGlobalBounds().height / 2.0f);
		icons[i].scale(0.5f, 0.5f);

		if (i > 0)
			icons[i].setScale(0.20f, 0.20f);
		
		switch (i) // i refering to 
		{
		case 0: // bar graph
			icons[i].setPosition(heat[0].getPosition().x + (heat[0].getGlobalBounds().width / 2.0f), heat[0].getPosition().y + (heat[0].getGlobalBounds().height / 2.0f));
			break;
		case 1: // save 
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width / 4.0f), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		case 2: // quit game
			icons[i].setPosition(heat[2].getPosition().x + (heat[2].getGlobalBounds().width * (3.0f / 4.0f)), heat[2].getPosition().y + (heat[2].getGlobalBounds().height / 2.0f));
			break;
		}
	}
}

void inventory::update_list()
{
	std::list<instrument *>::iterator it = current_user->get_active_store()->get_inventory()->begin();

	for (int i = 0; i < starting_index * 5; i++) it++;

	for (int i = 0; i < 5; i++) {
		currently_showing[i].setString(std::to_string(starting_index * 5 + i + 1) + ". " + (*it)->print_brand_cpp());
		it++;
		if (it == current_user->get_active_store()->get_inventory()->end()) {
			for (int j = i+1; j < 5; j++) {
				currently_showing[j].setString("");
			}
			break;
		}
	}
}