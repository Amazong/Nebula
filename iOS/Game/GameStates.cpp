#include "headers\GameStates.h"


/*------------------------------Main Menu------------------------------*/

main_menu::main_menu(state_manager * game_ptr)
{
	game = game_ptr; // pointer to state manager
	
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
				if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
					game->window.close();
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
					MUSIC::get_m_player()->set_skip(true);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
					if (MUSIC::get_m_player()->get_stop() == false) {
						MUSIC::get_m_player()->set_stop(true);
					}
					else MUSIC::get_m_player()->set_stop(false);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
					MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
					MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
				}
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
						game->push_state(new new_game(game, game->window.capture()));
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
				if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
					game->window.close();
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
					MUSIC::get_m_player()->set_skip(true);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
					if (MUSIC::get_m_player()->get_stop() == false) {
						MUSIC::get_m_player()->set_stop(true);
					}
					else MUSIC::get_m_player()->set_stop(false);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
					MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
				}
				else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
					MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
				}
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

	current_user = game_ptr->get_current_user();

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	setup_options();
	update_indicators();
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
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
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
				current_user->buy_store(new store(current_user, ""));
				game->push_state(new inventory(game, game->window.capture()));
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
	//update_buying_rate();
	//if (buying_rate > active_store->get_stock()) {
	//
	//	// if not enough items in stock, penalize player
	//	active_store->set_reputation (active_store->get_reputation() * 0.9);
	//}
	
	current_user->time_elapsed += sf::Time(sf::seconds(elapsed));
	buffer += sf::Time(sf::seconds(elapsed));

	if (buffer >= sf::Time(sf::seconds(current_user->WEEK_TIME_SECS))) {
		buffer -= sf::Time(sf::seconds(current_user->WEEK_TIME_SECS));
	}

	if ((int)(current_user->time_elapsed.asSeconds()) % (int)current_user->WEEK_TIME_SECS == 0) {
		if ((int)(current_user->time_elapsed.asSeconds()) != last_second) {
			last_second = (int)(current_user->time_elapsed.asSeconds());
			indicators_str[2] = current_user->get_time_str();
			add_profits(this->current_user->net_worth);
		}
	}
	
	update_indicators();
}

void in_game::draw(const float elapsed)
{
	for (int i = 0; i < 7; i++)
		game->window.draw(heat[i]);
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

void in_game::update_indicators()
{
	int offset = (int)(heat[1].getGlobalBounds().height / 5.0f);

	indicators[0].setString(current_user->get_balance_styled());
	double rep = current_user->get_reputation();

	if (rep < -0.75) // -1 -> -0.75
		indicators[1].setString("Comcast");
	else if (rep < -0.25) // -0.75 -> -0.25
		indicators[1].setString("NOS");
	else if (rep < 0.25) // -0.25 -> 0.25
		indicators[1].setString("Meh.");
	else if (rep < 0.75) // 0.25 -> 0.75
		indicators[1].setString("Google-like");
	else // > 0.75
		indicators[1].setString("Apple-like");

	indicators[2].setString(current_user->get_time_str());

	for (int i = 0; i < 5; i++)
	{
		indicators[i].setFont(options_font);
		indicators[i].setCharacterSize((int)(game->window.getSize().y / 22.0f));
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
		current_user->save_game();
		return(true);
	}
	return(false);
}

void in_game::add_profits(long double n_worth)
{
	past_net_worths.push_front(n_worth);

	while (past_net_worths.size() > 52) {
		past_net_worths.pop_back();
	}

	update_profits();	
}

void in_game::update_profits()
{
	// past 4 weeks
	long double average = 0;
	int lim = (past_net_worths.size() < 4) ? past_net_worths.size() : 4;
	for (int i = 0; i < lim; i++) {
		average += (past_net_worths.at(i) / (double)lim);
	}
	indicators[3].setString("AMP : " + style(average));

	// last year
	average = 0;
	lim = (past_net_worths.size() < 52) ? past_net_worths.size() : 52;
	for (int i = 0; i < lim; i++) {
		average += (past_net_worths.at(i) / (double)lim);
	}
	indicators[4].setString("AYP : " + style(average));
}

std::string in_game::style(long double d)
{
	std::string s = "";

	if (d < 0) { // negative
		s += "- ";
		s += style(-d);
		return s;
	}
	else if (d < 1000) { // display directly
		s += std::to_string((int)d).substr(0, 3);
	}
	else if (d < 1000000) { // K range
		s += std::to_string((int)d / 1000).substr(0, 3); // How many K's
		s += ".";
		s += std::to_string((int)d % 1000).substr(0, 1);
		s += "K";
	}
	else if (d < 1000000000) { // M range
		s += std::to_string((int)d / 1000000).substr(0, 3); // How many M's
		s += ".";
		s += std::to_string((int)d % 1000000).substr(0, 1);
		s += "M";
	}
	else {
		s += "1 $"; // if the user has more than 1000000000 £, we display "1 $"
					// small easter egg :)
		return s;
	}

	s += " £";

	return s;
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
	if (selection > 2 && selection != 9)
	{
		options[(selection - 3)].setStyle(sf::Text::Underlined);
		options[(selection - 3)].setColor(sf::Color::White);
		start_text.setScale(1.0f, 1.0f);
	}
	


	while (game->window.pollEvent(event))
	{
		icons[5].setScale(0.2f, 0.2f);
		icons[6].setScale(0.2f, 0.2f);
		start_text.setScale(1.0f, 1.0f);

		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);
			
			if (selection > 2 && selection != 9)
			{
				options[(selection - 3)].setScale(1.0f, 1.0f);
				options[(selection - 3)].setStyle(sf::Text::Regular);
			}

			if (selection != -1 && selection != 9)
				heat[selection].setOutlineThickness(0);

			selection = -1; // this way the selection will always be -1 if it's not in one of the options

			for (int i = 0; i < 4; i++)
			{
				if (heat[(3 + i)].getGlobalBounds().contains(mouse_pos))
				{
					selection = i + 3;
				}
			}
			if (start_text.getGlobalBounds().contains(mouse_pos) && !game->get_current_user()->stores.empty() && !game->get_current_user()->get_active_store()->staff.empty() && !game->get_current_user()->get_active_store()->inventory.empty()) //only let's you press when you have stuff ready!
				selection = 9;

			if (selection != -1 && selection != 9)
			{
				options[(selection - 3)].scale(1.1f, 1.1f);
				options[(selection - 3)].setStyle(sf::Text::Underlined);				
			}

			if (selection != 9)
			{
				start_text.setStyle(sf::Text::Regular);
				start_text.setScale(1.0f, 1.0f);
			}
			else
			{
				start_text.setStyle(sf::Text::Underlined);
				start_text.scale(1.1f, 1.1f);
			}

			control_icon_animations(mouse_pos);

			std::cout << "           Selection " << selection << std::endl; //debug


			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (handle_icons((sf::Vector2f) sf::Mouse::getPosition(game->window)))
				return;

			if (selection > 2 && selection != 9 && event.mouseButton.button == sf::Mouse::Left)
			{
				options[(selection - 3)].setStyle(sf::Text::Italic);
				options[(selection - 3)].setColor(sf::Color::Red);
			}

			switch (selection)
			{
				case 0:
				{
					game->pop_state();
					return;
					break;
				}
				case 9:
				{
					game->change_state(new in_game(game));
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
		
		return(true);
	}
	return(false);
}


/*------------------------------ New_Game1 ------------------------------*/

new_game::new_game(state_manager * game, sf::Image Background_img)
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

void new_game::input()
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
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);
			
			selection = -1; // this way the selection will always be -1 if it's not in one of the options

			for (int i = 2; i < 6; i++)
			{
				if (i == 5)
				{
					if (name.isEmpty())
						continue;
				}
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
					game->set_current_user(new user_profile());

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

					game->push_state(new msg_box(game, game->window.capture(), "Game created!",20, 50, new in_game_setup(game)));
					return;
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

void new_game::logic_update(const float elapsed)
{
}

void new_game::draw(const float elapsed)
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

void new_game::setup_options()
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
				options[i].move(0, (input_place.getGlobalBounds().height * 3.0f )/ 5.0f);
				break;
			}
			default:
			{
				options[i].move(0, (input_place.getGlobalBounds().height * 2.0f) / 5.0f);
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
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);

			selection = -1; // this way the selection will always be -1 if it's not in one of the options
			for (int i = 0; i < n_saves; i++) {
				saved_profiles[i].setColor(sf::Color(190, 190, 190, 255));
			}

			for (int i = 0; i < n_saves; i++)
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


/*------------------------------    msg_box    ------------------------------*/

msg_box::msg_box(state_manager * game_ptr, sf::Image background_img, std::string str, unsigned int line_size, unsigned int char_size, game_state * next_state)
{
	game = game_ptr;
	this->line_size = line_size;
	this->char_size = char_size;
	this->str = str;
	this->next_state = next_state;

	if (!background.loadFromImage(background_img)) {		//to deal with
		complain(ErrNo::file_access);
		return;
	}

	if (!options_font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}


	setup_text();

	background_sprite.setTexture(background);						//to deal with
	background_sprite.setScale(game->window.getSize().x / 1920.0f, game->window.getSize().y / 1080.0f);

	show_textbox(this->str, this->line_size, this->char_size); // function for message boxes., line size in chars.

	close.setPosition(box.getPosition().x, box.getPosition().y + (box.getGlobalBounds().height / (3.6f)));
}

void msg_box::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default 

	while (game->window.pollEvent(event))
	{
		close.setScale(1.0f, 1.0f);
		close.setStyle(sf::Text::Regular);

		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);

			selection = -1; // this way the selection will always be -1 if it's not in one of the options
			close.setScale(1.0f, 1.0f);
			close.setStyle(sf::Text::Regular);
			if (close.getGlobalBounds().contains(mouse_pos))
			{
				selection = 1;
				close.setScale(1.1f, 1.1f);
				close.setStyle(sf::Text::Underlined);
			}
			else if (box.getGlobalBounds().contains(mouse_pos))
				selection = 0;

			std::cout << "           Selection " << selection << std::endl; //debug
			
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				switch (selection)
				{
					case -1:
					{
						if (next_state != nullptr) {
							delete[] options;

							state_manager * aux = game;
							game_state * next = next_state;
							aux->pop_state();
							aux->change_state(next);
							return;
						}

						game->pop_state();
						return;
					}
					case 1:
					{
						if (next_state != nullptr) {
							delete[] options;

							state_manager * aux = game;
							game_state * next = next_state;
							aux->pop_state();
							aux->change_state(next);
							return;
						}
						
						game->pop_state();
						return;
					}
					default:
						break;
				}

				
			}
			break;
		}
		default:
			break;
		}
	}


}

void msg_box::logic_update(const float elapsed)
{

}

void msg_box::draw(const float elapsed)
{
	game->window.draw(background_sprite);
	game->window.draw(box);
	for (int i = 0; i < text_size; i++)
	{
		game->window.draw(options[i]);
	}

	game->window.draw(close);
}

void msg_box::show_textbox(std::string & str, unsigned int line_size, unsigned int char_size)
{
	float offset = 5;
	
	std::string * placeholder = get_string_tab(str, text_size, line_size);

	options = new sf::Text[text_size];
	
	sf::Vector2f window_size(((line_size * char_size) / 2.0f) + (char_size * 3.0f), (char_size * 3.0f) + (text_size * char_size) + ((text_size - 1.0f) * offset));

	box.setSize(sf::Vector2f(window_size.x, window_size.y + close.getGlobalBounds().height * 1.4f));
	box.setFillColor(sf::Color(21, 21, 21, 255));
	
	box.setOrigin(box.getGlobalBounds().width / 2.0f, box.getGlobalBounds().height / 2.0f);
	box.setPosition(game->window.getSize().x * 0.50f, game->window.getSize().y * 0.50f);
	box.setOutlineThickness(-3);
	box.setOutlineColor(sf::Color(72, 72, 72, 255));

	sf::Vector2f pos(box.getPosition().x, box.getPosition().y - (box.getGlobalBounds().height / (2.8f)));

	for (int i = 0; i < text_size; i++, pos.y += offset)
	{
		options[i].setFont(options_font);
		options[i].setCharacterSize(char_size);
		options[i].setString(placeholder[i]);
		options[i].setColor(sf::Color::White);
		options[i].setOrigin((options[i].getGlobalBounds().width / 2.0f), (options[i].getGlobalBounds().height / 2.0f)); // origin in center.
		options[i].setPosition(pos);

		pos.y += char_size;
	}
	
	(text_size == 1) ? delete placeholder : delete[] placeholder; //if size == 1 i used new, not new[]
}

void msg_box::setup_text()
{
	


	close.setFont(options_font);
	close.setCharacterSize(char_size);
	close.setString("Close");
	close.setColor(sf::Color::White);
	close.setOrigin((close.getGlobalBounds().width / 2.0f), (close.getGlobalBounds().height / 2.0f)); // origin of font in its geometric center

}


/*------------------------------ Inventory ------------------------------*/

inventory::inventory(state_manager * game_ptr, sf::Image print)
{
	game = game_ptr;
	in_game_printscr = print;

	if (!font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	if (!backgroud_texture.loadFromImage(print)) {
		complain(ErrNo::file_access);
		return;
	}

	background.setTexture(backgroud_texture, true);
	background.setTextureRect(sf::IntRect(0, 0, (int)(game->window.getSize().x / 5.8f), (int)(game->window.getSize().y - (game->window.getSize().y / 7.0f))));

	details.setFillColor(sf::Color::Color(170, 170, 170, 235));
	details.setSize(sf::Vector2f((8.0f / 16.0f) * game->window.getSize().x, (game->window.getSize().y * (2.0f / 3.0f))));
	details.setPosition(game->window.getSize().x * (7.0f / 16.0f), game->window.getSize().y / 5.5f);
	details.setOutlineColor(sf::Color(100, 100, 100, 255));
	details.setOutlineThickness(-3);

	price_setter.setFillColor(sf::Color::Transparent);

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

		// if in input mode
		if (price_setter.getFillColor() == sf::Color::White) {
			if (event.type == sf::Event::TextEntered)
			{
				if (price_setter_str.length() > 50 || price_setter_inside.getGlobalBounds().width >= (price_setter.getGlobalBounds().width - 30))
				{
					if (event.text.unicode == 8) // backspace
					{
						if (price_setter_inside.getString().isEmpty()) break;
						price_setter_str.erase(price_setter_str.length() - 1);
						price_setter_inside.setString(price_setter_str + " £");
					}
					else if (event.text.unicode == 13) { // return
						price_setter.setFillColor(sf::Color::Transparent);
						set_price.setColor(sf::Color(70, 70, 70, 255));

						current_selection->set_price(atof(price_setter_str.c_str()));

						update_properties();
					}
					else if (event.text.unicode == 27) { // escape
						price_setter.setFillColor(sf::Color::Transparent);
						price_setter_inside.setString("");
						set_price.setColor(sf::Color(70, 70, 70, 255));
					}
				}
				else if (event.text.unicode == 8) // backspace
				{
					try {
						price_setter_str.at(0);
						price_setter_str.erase(price_setter_str.length() - 1);
						price_setter_inside.setString(price_setter_str + " £");
					}
					catch (const std::out_of_range) {
						std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
						break; // otherwise, continue accepting numbers
					}					
				}
				else if (event.text.unicode > 47 && event.text.unicode < 58) // numbers
				{
					int length = price_setter_str.length();

					for (int i = 0; i < length; i++) {
						if (price_setter_str.at(i) == '.') { // if there is a decimal point already
							try {
								price_setter_str.at(i + 2); // if the number is fully written, return
							}
							catch (const std::out_of_range) {
								break; // otherwise, continue accepting numbers
							}
							return; // number has two decimal places
						}
					}
					
					price_setter_str += static_cast<char>(event.text.unicode);
					price_setter_inside.setString(price_setter_str + " £");
				}
				else if (event.text.unicode == 46) // decimal point
				{
					int length = price_setter_str.length();

					if (length == 0) {
						price_setter_str = "0.";
						price_setter_inside.setString(price_setter_str + " £");
						return;
					}

					for (int i = 0; i < length; i++) {
						if (price_setter_str.at(i) == '.') return;
					}
					price_setter_str += static_cast<char>(event.text.unicode);
					price_setter_inside.setString(price_setter_str + " £");
				}
				else if (event.text.unicode == 13) { // return
					price_setter.setFillColor(sf::Color::Transparent);
					set_price.setColor(sf::Color(70, 70, 70, 255));

					current_selection->set_price(atof(price_setter_str.c_str()));

					update_properties();
				}
				else if (event.text.unicode == 27) { // escape
					price_setter.setFillColor(sf::Color::Transparent);
					price_setter_inside.setString("");
					set_price.setColor(sf::Color(70, 70, 70, 255));
				}
			}
			return;
		}

		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);

			// update properties
			for (int i = 0; i < 5; i++) {
				if (currently_showing[i].getGlobalBounds().contains(mouse_pos) && currently_showing[i].getString() != "") {
					int j;
					int item_number = starting_index * 5 + i;
					auto it = active_store->get_inventory()->begin();

					currently_showing[i].setStyle(sf::Text::Bold);

					for (j = 0; j < 5; j++) {
						if (j == i) continue;
						currently_showing[j].setStyle(sf::Text::Regular);
					}

					for (j = 0; j < item_number; j++) {
						it++;
					}

					current_selection = *it;
					
					set_price.setColor(sf::Color(70, 70, 70, 255));

					update_properties();
					return;
				}
			}

			if (back.getGlobalBounds().contains(mouse_pos)) {
				selection = 0;
				back.setStyle(sf::Text::Bold);
			}
			else if (buy.getGlobalBounds().contains(mouse_pos)) {
				selection = 1;
				buy.setStyle(sf::Text::Bold);
			}
			else if (set_price.getGlobalBounds().contains(mouse_pos)) {
				selection = 2;
			}
			else if (icons[1].getGlobalBounds().contains(mouse_pos)) {
				icons[1].setScale(0.3f, 0.3f);
				selection = 3;
			}
			else if (icons[2].getGlobalBounds().contains(mouse_pos)) {
				icons[2].setScale(0.3f, 0.3f);
				selection = 4;
			}
			else if (scroll[0].getGlobalBounds().contains(mouse_pos)) {
				scroll[0].setScale(0.5f, 0.5f);
				selection = 5;
			}
			else if (scroll[1].getGlobalBounds().contains(mouse_pos)) {
				scroll[1].setScale(0.5f, 0.5f);
				selection = 6;
			}
			else {
				icons[1].setScale(0.2f, 0.2f);
				icons[2].setScale(0.2f, 0.2f);
				scroll[0].setScale(0.4f, 0.4f);
				scroll[1].setScale(0.4f, 0.4f);
				selection = -1;
				buy.setStyle(sf::Text::Regular);
				back.setStyle(sf::Text::Regular);
			}			

			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			state_manager * aux = game;
			switch (selection) // 0 - back; 1 - buy; 2 - set price; 3 - save; 4 - exit; 5 - move down; 6 - move up
			{
			case 0:
				game->pop_state();
				return;
			case 1:
				game->change_state(new inventory_buy(game, in_game_printscr));
				return;
			case 2:
				if (price_setter.getFillColor() == sf::Color::Transparent &&
					set_price.getColor() != sf::Color::Transparent) {
					price_setter.setFillColor(sf::Color::White);
					set_price.setColor(sf::Color::Transparent);
					price_setter_str = "";
				}
				return;
			case 3:
				// save game
				return;
			case 4:
				aux->pop_state();
				aux->pop_state();
				return;
			case 5:
				if (starting_index != current_user->get_active_store()->get_stock() / 5) {
					move_list_down();
					scroll[1].setScale(0.4f, 0.4f);
				}
				break;
			case 6:
				if (starting_index != 0)
					move_list_up();
					scroll[0].setScale(0.4f, 0.4f);
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

void inventory::logic_update(const float elapsed)
{

}

void inventory::draw(const float elapsed)
{
	game->window.draw(background);
	game->window.draw(details);
	game->window.draw(buy);
	game->window.draw(back);
	game->window.draw(set_price);
	game->window.draw(price_setter);
	game->window.draw(price_setter_inside);
	
	if (starting_index == current_user->get_active_store()->get_stock() / 5)
		scroll[0].setScale(0.0f, 0.0f);

	if (starting_index == 0)
		scroll[1].setScale(0.0f, 0.0f);
			
	int i;
	for (i = 0; i < 2; i++)
		game->window.draw(scroll[i]);

	for (i = 0; i < 3; i++)
		game->window.draw(icons[i]);
	
	for (i = 0; i < 5; i++)
		game->window.draw(currently_showing[i]);

	for (i = 0; i < 6; i++)
		game->window.draw(active_properties[i]);
}

void inventory::move_list_down()
{
	starting_index++;

	update_list();
	current_selection = nullptr;
	set_price.setColor(sf::Color::Transparent);
	update_properties();
}

void inventory::move_list_up()
{
	starting_index--;

	update_list();
	current_selection = nullptr;
	set_price.setColor(sf::Color::Transparent);
	update_properties();
}

void inventory::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();

	setup_text();
	setup_icons();
}

void inventory::setup_text()
{
	// currently showing (list)
	for (int i = 0; i < 5; i++) {
		currently_showing[i].setFont(font);
		currently_showing[i].setCharacterSize((int)(game->window.getSize().y / 16.0f));
		currently_showing[i].setColor(sf::Color::White);
		currently_showing[i].setPosition((game->window.getSize().x / 5.8f) + 90, (2 * i * currently_showing[i].getCharacterSize() + game->window.getSize().y / 5.0f));
	}
	update_list();

	// buy
	{
		buy.setFont(font);
		buy.setColor(sf::Color::White);
		buy.setCharacterSize(50);
		buy.setString("Buy Inventory");
		buy.setPosition((game->window.getSize().x - buy.getGlobalBounds().width - 50),
			game->window.getSize().y - (float)2 * buy.getCharacterSize());
	}

	// back
	{
		back.setFont(font);
		back.setColor(sf::Color::White);
		back.setCharacterSize(50);
		back.setString("Back");
		back.setPosition((game->window.getSize().x / 5.8f) + 50,
			game->window.getSize().y - (float)2 * buy.getCharacterSize());
	}

	// set price
	{
		set_price.setFont(font);
		set_price.setColor(sf::Color::Transparent);
		set_price.setString("Set price");
	}
}

void inventory::setup_icons()
{
	std::string scroll_names[2] = { "down_arrow.png", "up_arrow.png" };

	for (int i = 0; i < 2; i++)
	{
		if (!scroll_texture[i].loadFromFile("res/png/" + scroll_names[i]))
		{
			complain(ErrNo::file_access);
			return;
		}

		scroll[i].setTexture(scroll_texture[i]);
		scroll[i].setOrigin((scroll[i].getGlobalBounds().width / 2.0f), (scroll[i].getGlobalBounds().height / 2.0f));
		scroll[i].setScale(0.4f, 0.4f);	

	}

	scroll[0].setPosition(10 + currently_showing[0].getPosition().x + scroll[0].getGlobalBounds().width / 2.0f,
		currently_showing[0].getPosition().y - (1.0f / 2.0f)*scroll[0].getGlobalBounds().height);
	scroll[1].setPosition(currently_showing[0].getPosition().x + currently_showing[0].getGlobalBounds().width - scroll[1].getGlobalBounds().width - 10 + scroll[1].getGlobalBounds().width / 2.0f,
		currently_showing[0].getPosition().y - (1.0f / 2.0f)*scroll[1].getGlobalBounds().height);
}

void inventory::update_list()
{
	int i;
	std::list<instrument *>::iterator it = current_user->get_active_store()->get_inventory()->begin();

	for (i = 0; i < starting_index * 5; i++) it++;

	for (i = 0; i < 5; i++) {
		currently_showing[i].setString(std::to_string(starting_index * 5 + i + 1) + ". " + (*it)->print_brand_cpp_short());
		it++;
		if (it == current_user->get_active_store()->get_inventory()->end()) {
			for (int j = i+1; j < 5; j++) {
				currently_showing[j].setString("");
			}
			break;
		}
	}
}

void inventory::update_properties() {
	if (current_selection == nullptr) {
		for (int i = 0; i < 6; i++) {
			active_properties[i].setString("");
		}
		return;
	}	
	
	// active_properties
	int i = (current_selection->is_guitar) ? 3 : 5;
	
	if (i == 3) {
		active_properties[4].setString("");
		active_properties[5].setString("");
	}

	float base_size = (details.getGlobalBounds().height) / 19.0f;
	unsigned int character_size = (int)(2 * base_size);

	for (; i >= 0; i--) {
		active_properties[i].setFont(font);
		active_properties[i].setCharacterSize(character_size);
		active_properties[i].setColor(sf::Color::White);
		active_properties[i].setPosition(details.getPosition().x + 30,
			details.getPosition().y + base_size + i * 3 * base_size);

		switch (i) {
		case 0:
			active_properties[i].setString("Value: " + current_selection->style(current_selection->get_value()));
			break;
		case 1:
			active_properties[i].setString("Price: " + current_selection->style(current_selection->get_price()));
			
			set_price.setCharacterSize(active_properties[1].getCharacterSize());
			set_price.setPosition(active_properties[1].getPosition());
			set_price.setPosition(details.getPosition().x + details.getGlobalBounds().width - 30 - set_price.getGlobalBounds().width,
				set_price.getPosition().y);

			price_setter.setFillColor(sf::Color::Transparent);
			price_setter.setPosition(set_price.getPosition());
			price_setter.setSize(sf::Vector2f((float)set_price.getGlobalBounds().width, (float)set_price.getGlobalBounds().height));

			// price setting text
			price_setter_inside.setFont(font);
			price_setter_inside.setColor(sf::Color::Black);
			price_setter_inside.setString("");
			price_setter_inside.setPosition(price_setter.getPosition().x + 5, price_setter.getPosition().y + 10);
			
			break;
		case 2:
			active_properties[i].setString("Perceived value: " + current_selection->get_perceived_value_cpp());

			if (current_selection->get_perceived_value_cpp() == "Unattainable") active_properties[i].setCharacterSize(character_size - 10);
			else active_properties[i].setCharacterSize(character_size);

			break;
		case 3:
			active_properties[i].setString("Quality: " + current_selection->get_quality_cpp());
			break;
		case 4:
			active_properties[i].setString("Brand: " + current_selection->print_brand_cpp());
			break;
		case 5:
			active_properties[i].setString("Type: " + current_selection->print_type_cpp());
			break;
		default:
			break;
		}
	}
}


/*---------------------------- Inventory_Buy ----------------------------*/

inventory_buy::inventory_buy(state_manager * game_ptr, sf::Image print)
{
	game = game_ptr;

	if (!font.loadFromFile("res/fonts/Roboto-Bold.ttf")) {
		complain(ErrNo::file_access);
		return;
	}

	if (!backgroud_texture.loadFromImage(print)) {
		complain(ErrNo::file_access);
		return;
	}

	background.setTexture(backgroud_texture, true);
	background.setTextureRect(sf::IntRect(0, 0, (int)(game->window.getSize().x / 5.8f), (int)(game->window.getSize().y - (game->window.getSize().y / 7.0f))));

	details.setFillColor(sf::Color::Color(170, 170, 170, 235));
	details.setSize(sf::Vector2f((8.0f / 16.0f) * game->window.getSize().x, (game->window.getSize().y * (2.0f / 3.0f))));
	details.setPosition(game->window.getSize().x * (7.0f / 16.0f), game->window.getSize().y / 5.5f);
	details.setOutlineColor(sf::Color(100, 100, 100, 255));
	details.setOutlineThickness(-3);

	price_setter.setFillColor(sf::Color::Transparent);

	setup();
}

void inventory_buy::input()
{
	sf::Event event;
	sf::Vector2f mouse_pos(0.0f, 0.0f); // by default

	while (game->window.pollEvent(event))
	{
		icons[5].setScale(0.2f, 0.2f);
		icons[6].setScale(0.2f, 0.2f);

		// if in input mode
		if (price_setter.getFillColor() == sf::Color::White) {
			if (event.type == sf::Event::TextEntered)
			{
				if (price_setter_str.length() > 50 || price_setter_inside.getGlobalBounds().width >= (price_setter.getGlobalBounds().width - 30))
				{
					if (event.text.unicode == 8) // backspace
					{
						if (price_setter_inside.getString().isEmpty()) break;
						price_setter_str.erase(price_setter_str.length() - 1);
						price_setter_inside.setString(price_setter_str + " £");
					}
					else if (event.text.unicode == 13) { // return
						price_setter.setFillColor(sf::Color::Transparent);
						buy_selection.setColor(sf::Color(70, 70, 70, 255));

						current_selection->set_price(atof(price_setter_str.c_str()));

						update_properties();
					}
					else if (event.text.unicode == 27) { // escape
						price_setter.setFillColor(sf::Color::Transparent);
						price_setter_inside.setString("");
						buy_selection.setColor(sf::Color(70, 70, 70, 255));
					}
				}
				else if (event.text.unicode == 8) // backspace
				{
					try {
						price_setter_str.at(0);
						price_setter_str.erase(price_setter_str.length() - 1);
						price_setter_inside.setString(price_setter_str + " £");
					}
					catch (const std::out_of_range) {
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						break; // otherwise, continue accepting numbers
					}
				}
				else if (event.text.unicode > 47 && event.text.unicode < 58) // numbers
				{
					int length = price_setter_str.length();

					for (int i = 0; i < length; i++) {
						if (price_setter_str.at(i) == '.') { // if there is a decimal point already
							try {
								price_setter_str.at(i + 2); // if the number is fully written, return
							}
							catch (const std::out_of_range) {
								break; // otherwise, continue accepting numbers
							}
							return; // number has two decimal places
						}
					}

					price_setter_str += static_cast<char>(event.text.unicode);
					price_setter_inside.setString(price_setter_str + " £");
				}
				else if (event.text.unicode == 46) // decimal point
				{
					int length = price_setter_str.length();

					if (length == 0) {
						price_setter_str = "0.";
						price_setter_inside.setString(price_setter_str + " £");
						return;
					}

					for (int i = 0; i < length; i++) {
						if (price_setter_str.at(i) == '.') return;
					}
					price_setter_str += static_cast<char>(event.text.unicode);
					price_setter_inside.setString(price_setter_str + " £");
				}
				else if (event.text.unicode == 13) { // return
					price_setter.setFillColor(sf::Color::Transparent);
					buy_selection.setColor(sf::Color(70, 70, 70, 255));

					current_selection->set_price(atof(price_setter_str.c_str()));

					update_properties();
				}
				else if (event.text.unicode == 27) { // escape
					price_setter.setFillColor(sf::Color::Transparent);
					price_setter_inside.setString("");
					buy_selection.setColor(sf::Color(70, 70, 70, 255));
				}
			}
			return;
		}

		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (event.key.alt && (event.key.code == sf::Keyboard::F4)) {
				game->window.close();
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::S)) {
				MUSIC::get_m_player()->set_skip(true);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::M)) {
				if (MUSIC::get_m_player()->get_stop() == false) {
					MUSIC::get_m_player()->set_stop(true);
				}
				else MUSIC::get_m_player()->set_stop(false);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::J)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() - 5);
			}
			else if (event.key.alt && (event.key.code == sf::Keyboard::K)) {
				MUSIC::get_m_player()->set_MAX_VOL(MUSIC::get_m_player()->get_MAX_VOL() + 5);
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(game->window);

			// update properties
			for (int i = 0; i < 5; i++) {
				if (currently_showing[i].getGlobalBounds().contains(mouse_pos) && currently_showing[i].getString() != "") {
					int j;
					int item_number = starting_index * 5 + i;
					auto it = purchaseable.begin();

					currently_showing[i].setStyle(sf::Text::Bold);

					for (j = 0; j < 5; j++) {
						if (j == i) continue;
						currently_showing[j].setStyle(sf::Text::Regular);
					}

					for (j = 0; j < item_number; j++) {
						it++;
					}

					current_selection = *it;

					buy_selection.setColor(sf::Color(70, 70, 70, 255));

					update_properties();
					return;
				}
			}

			if (back.getGlobalBounds().contains(mouse_pos)) {
				selection = 0;
				back.setStyle(sf::Text::Bold);
			}
			else if (buy_selection.getGlobalBounds().contains(mouse_pos)) {
				selection = 2;
			}
			else if (icons[1].getGlobalBounds().contains(mouse_pos)) {
				icons[1].setScale(0.3f, 0.3f);
				selection = 3;
			}
			else if (icons[2].getGlobalBounds().contains(mouse_pos)) {
				icons[2].setScale(0.3f, 0.3f);
				selection = 4;
			}
			else {
				icons[1].setScale(0.2f, 0.2f);
				icons[2].setScale(0.2f, 0.2f);
				selection = -1;
				back.setStyle(sf::Text::Regular);
			}

			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			state_manager * aux = game;
			switch (selection) // 0 - back; 1 - buy; 2 - set price; 3 - save; 4 - exit; 5 - move down; 6 - move up
			{
			case 0:
				game->pop_state();
				return;
			case 1:
				// game->change_state(new inventory_buy_buy(game));
				return;
			case 2:
				if (price_setter.getFillColor() == sf::Color::Transparent &&
					buy_selection.getColor() != sf::Color::Transparent) {
					price_setter.setFillColor(sf::Color::White);
					buy_selection.setColor(sf::Color::Transparent);
					price_setter_str = "";
				}
				return;
			case 3:
				// save game
				return;
			case 4:
				aux->pop_state();
				aux->pop_state();
				return;
			case 5:
				move_list_down();
				break;
			case 6:
				move_list_up();
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

void inventory_buy::logic_update(const float elapsed)
{

}

void inventory_buy::draw(const float elapsed)
{
	game->window.draw(background);
	game->window.draw(details);
	game->window.draw(back);
	game->window.draw(buy_selection);
	game->window.draw(price_setter);
	game->window.draw(price_setter_inside);

	int i;

	for (i = 0; i < 3; i++)
		game->window.draw(icons[i]);

	for (i = 0; i < 5; i++)
		game->window.draw(currently_showing[i]);

	for (i = 0; i < 6; i++)
		game->window.draw(active_properties[i]);
}

void inventory_buy::move_list_down()
{
	starting_index++;

	update_list();
	current_selection = nullptr;
	buy_selection.setColor(sf::Color::Transparent);
	update_properties();
}

void inventory_buy::move_list_up()
{
	starting_index--;

	update_list();
	current_selection = nullptr;
	buy_selection.setColor(sf::Color::Transparent);
	update_properties();
}

void inventory_buy::setup()
{
	current_user = game->get_current_user();
	active_store = current_user->get_active_store();

	setup_text();
	setup_purchaseable();
}

void inventory_buy::setup_text()
{
	// currently showing (list)
	for (int i = 0; i < 5; i++) {
		currently_showing[i].setFont(font);
		currently_showing[i].setCharacterSize((int)(game->window.getSize().y / 16.0f));
		currently_showing[i].setColor(sf::Color::White);
		currently_showing[i].setPosition((game->window.getSize().x / 5.8f) + 90, (2 * i * currently_showing[i].getCharacterSize() + game->window.getSize().y / 5.0f));
	}
	update_list();
	
	// back
	{
		back.setFont(font);
		back.setColor(sf::Color::White);
		back.setCharacterSize(50);
		back.setString("Back");
		back.setPosition((game->window.getSize().x / 5.8f) + 50,
			game->window.getSize().y - (float)2 * back.getCharacterSize());
	}

	// buy_selection
	{
		buy_selection.setFont(font);
		buy_selection.setColor(sf::Color::Transparent);
		buy_selection.setString("Buy");
	}
}

void inventory_buy::setup_purchaseable()
{
	guitar * new_guitar;
	piano * new_piano;
	int sel;

	std::random_device rd;
	std::mt19937 random_numbers(rd());
	std::uniform_int_distribution<int> range(0, 1);

	for (int i = 0; i < 5; i++) {
		sel = range(random_numbers);
		if (sel == 0) {
			do {
				new_guitar = new guitar("");
			} while (new_guitar->get_value() > current_user->get_balance());
			purchaseable.push_back(new_guitar);
		}
		else {
			do {
				new_piano = new piano("");
			} while (new_piano->get_value() > current_user->get_balance());
			purchaseable.push_back(new_piano);
		}
	}
}

void inventory_buy::update_list()
{
	int i;
	std::list<instrument *>::iterator it = purchaseable.begin();

	for (i = 0; i < starting_index * 5; i++) it++;

	for (i = 0; i < 5; i++) {
		currently_showing[i].setString(std::to_string(starting_index * 5 + i + 1) + ". " + (*it)->print_brand_cpp_short());
		it++;
		if (it == purchaseable.end()) {
			for (int j = i + 1; j < 5; j++) {
				currently_showing[j].setString("");
			}
			break;
		}
	}
}

void inventory_buy::update_properties() {
	if (current_selection == nullptr) {
		for (int i = 0; i < 6; i++) {
			active_properties[i].setString("");
		}
		return;
	}

	// active_properties
	int i = (current_selection->is_guitar) ? 3 : 5;

	if (i == 3) {
		active_properties[4].setString("");
		active_properties[5].setString("");
	}

	float base_size = (details.getGlobalBounds().height) / 19.0f;
	unsigned int character_size = (int)(2 * base_size);

	for (; i >= 0; i--) {
		active_properties[i].setFont(font);
		active_properties[i].setCharacterSize(character_size);
		active_properties[i].setColor(sf::Color::White);
		active_properties[i].setPosition(details.getPosition().x + 30,
			details.getPosition().y + base_size + i * 3 * base_size);

		switch (i) {
		case 0:
			active_properties[i].setString("Value: " + current_selection->style(current_selection->get_value()));
			break;
		case 1:
			active_properties[i].setString("Quality: " + current_selection->get_quality_cpp());
			break;
		case 2:
			active_properties[i].setString("Brand: " + current_selection->print_brand_cpp());
			break;
		case 3:
			active_properties[i].setString("Type: " + current_selection->print_type_cpp());
			break;
		default:
			break;
		}
	}
}
