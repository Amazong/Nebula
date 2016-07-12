#pragma once
#include <string>
#include <iostream>
//for debugging only
#include <thread>
#include "StateManager.h"
#include "SFML\Graphics.hpp"
#include "Music.h"
#include "GameMechanics.h"
#include "Errors.h"
#include "GameWindows.h"


class game_state  // base class common to all derived states.
{
private:
	bool ok = true; // by default, everything is fine
	int error_code = -1; // by default, everything is fine
	void make_ok() { ok = true; }

	friend class state_manager;

public:
	state_manager *game;

	bool is_ok() { return ok; }
	int get_error_code() { return error_code; }
	void complain(int err = -1) { ok = false; error_code = err; game->window.setVisible(0); }

	virtual void input() = 0;
	virtual void logic_update(const float elapsed) = 0;
	virtual void draw(const float elapsed) = 0;

	friend class main_menu;
	friend class options_menu;
	friend class in_game;
};

/*------------------------------ Main Menu ------------------------------*/

class main_menu : public game_state
{
private:
	const std::string options_text[4] = { "New Game", "Continue", "Options", "Quit" };
	bool save_game;
	bool animation;
	double target_size;
	double t = 0;
	sf::Clock t_clock;
	sf::Font menu_font;
	sf::Text options[4];
	sf::Texture background;
	sf::Sprite background_sprite;
	sf::Vector2u current_size;
	sf::RectangleShape selector;
	int selection;
	int selection_old;

public:
	main_menu(state_manager * game);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	void setup_text();
	void update_save(); // TO-DO - checks if there is a *valid* saved game; menu is displayed accordingly.
};

/*------------------------------ Options Menu ------------------------------*/

class options_menu : public game_state
{
private:
	const std::string options_str[3] = { "Back", "Music", "Difficulty"};
	sf::Font options_font;
	sf::Text options[3];
	sf::Texture background;
	sf::Sprite background_sprite;
	sf::Sprite selector;
	sf::Texture selector_text;
	int selection = -1; // by default nothing is selected

public:
	options_menu(state_manager * game_ptr);
	
	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	
	void setup_text();
};

/*------------------------------ InGame ------------------------------*/

class in_game : public game_state
{
private:
	const std::string options_str[4] = { "Staff", "Inventory", "Shop", "Finance" };
	const std::string indicators_str[5] = {"Balance","Reputation","Game Time","Month's profits","Year's Profits"};
	sf::Font options_font;
	sf::Text options[4];
	sf::Text indicators[5];
	sf::RectangleShape heat[7];
	sf::Sprite icons[7];
	sf::Texture icons_texture[7];
	double buying_rate;
	int selection = -1;
	store * active_store;
	user_profile * current_user;
	void update_buying_rate();

public:
	in_game(state_manager * game_ptr);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void setup();
	void setup_options();
	void setup_indicators();
	void setup_icons();
	void control_icon_animations(sf::Vector2f mouse_pos);
	bool handle_icons(sf::Vector2f mouse_pos);
};

/*---------------------------- InGameSetup ----------------------------*/

class in_game_setup : public game_state
{
private:
	const std::string options_str[4] = { "Staff", "Inventory", "Shop", "Finance" };
	const std::string indicators_str[5] = { "Balance","Reputation","Game Time","Month's profits","Year's Profits" };
	sf::Font options_font;
	sf::Text start_text;
	sf::Text options[4];
	sf::Text indicators[5];
	sf::RectangleShape heat[7];
	sf::Sprite icons[7];
	sf::Texture icons_texture[7];
	double buying_rate;
	int selection = -1;
	store * active_store;
	user_profile * current_user;
	void update_buying_rate();

public:
	in_game_setup(state_manager * game_ptr);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void setup();
	void setup_options();
	void setup_indicators();
	void setup_icons();
	void control_icon_animations(sf::Vector2f mouse_pos);
	bool handle_icons(sf::Vector2f mouse_pos);
};


/*------------------------------ New_Game1 ------------------------------*/

class new_game1 : public game_state
{
private:
	const std::string options_str[6] = { "Username:", "Difficulty:", "Easy", "Medium" ,"Hard", "Continue" };
	sf::Font options_font;
	sf::String name = "";
	sf::Text options[7];
	sf::Text name_text;
	sf::Texture background;
	sf::Sprite background_sprite;
	sf::RectangleShape text_enter;
	sf::RectangleShape input_place;
	int difficulty = 0; // 0 to 2
	int selection = -1;
public:
	new_game1(state_manager * game, sf::Image Background);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	void setup_options();
};

/*------------------------------ Continue ------------------------------*/

class continue_game : public game_state
{
private:
	sf::Font font;
	sf::Text title;
	sf::Text saved_profiles[5];
	int n_saves;
	sf::Texture background_texture;
	sf::Sprite background_sprite;

	sf::RectangleShape continue_background;

	int selection = -1; // by default nothing is selected

public:
	continue_game(state_manager * game_ptr, sf::Image background);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void setup_text();
};