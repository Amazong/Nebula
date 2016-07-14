#pragma once
#include <string>
#include <iostream>
//for debugging only
#include <thread>
#include <deque>
#include <list>
#include "StateManager.h"
#include "SFML\Graphics.hpp"
#include "Music.h"
#include "GameMechanics.h"
#include "Errors.h"
#include "GameWindows.h"


class game_state // base class common to all derived states.
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
	const std::string options_str[3] = { "Back", "Stop Music", "Skip Song"};
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
	std::string indicators_str[5] = { "Balance", "Reputation", "Game Time", "Month's profits", "Year's Profits" };

	sf::Time buffer = sf::seconds(0.0f);

	std::deque<long double> past_net_worths;
	
	sf::Font options_font;
	sf::Text options[4];
	sf::Text indicators[5];
	sf::RectangleShape heat[7];
	sf::Sprite icons[7];
	sf::Texture icons_texture[7];

	int sold_this_week;
	int rev_this_week;

	int selection = -1;
	int last_second = -1;

	store * active_store;
	user_profile * current_user;
	
	void process_sales();

public:
	in_game(state_manager * game_ptr);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void setup();
	void setup_options();
	void update_indicators();
	void setup_icons();
	void control_icon_animations(sf::Vector2f mouse_pos);
	bool handle_icons(sf::Vector2f mouse_pos);

	void add_profits(long double n_worth);
	void update_profits();
	
	std::string style(long double d);
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
	void process_sales();

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


/*------------------------------ New_Game ------------------------------*/

class new_game : public game_state
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
	new_game(state_manager * game, sf::Image Background);

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

/*------------------------------    msg_box    ------------------------------*/


class msg_box : public game_state
{
private:
	std::string str;
	game_state * next_state;
	unsigned int line_size;
	unsigned int char_size;
	sf::Text close;
	sf::Font options_font;
	sf::Text * options;
	sf::Texture background;
	sf::Sprite background_sprite;
	sf::RectangleShape box;
	int text_size;
	int selection = -1; // by default nothing is selected

public:
	msg_box(state_manager * game_ptr, sf::Image background_img, std::string str, unsigned int line_size, unsigned int char_size, game_state * next_state = nullptr);
	
	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void show_textbox(std::string & str, unsigned int line_size, unsigned int char_size); // function for message boxes., line size in chars.
	void setup_text();
};

//sf::Vector2f starting_pos(game->window.getSize().x * 0.50f, game->window.getSize().y * 0.50f);
//
//close.setPosition(starting_pos);


/*------------------------------ Inventory ------------------------------*/

class inventory : public game_state
{
private:
	sf::Image in_game_printscr;

	sf::Font font;
	sf::Text title;
	sf::Text buy;
	sf::Text back;
	sf::Text set_price;
	sf::Text price_setter_inside;
	sf::Text active_properties[6]; // value, price, perceived value, quality, brand, type (if piano)
	sf::Text currently_showing[5];
	
	std::string price_setter_str;

	sf::RectangleShape details;
	sf::RectangleShape price_setter;
	
	sf::Sprite scroll[2];
	sf::Texture scroll_texture[2];
	
	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	int starting_index = 0;
	instrument * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_icons();
	void update_list();
	void update_properties();

public:
	inventory(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	void move_list_down();
	void move_list_up();
};


/*------------------------------  Finance  ------------------------------*/

class finance : public game_state
{
private:
	std::string options_str[2] = {"Stores:", "Back"};
	std::string indicators_str[5] = { "Balance", "Reputation", "Game Time", "Month's profits", "Year's Profits" };

	sf::Time buffer = sf::seconds(0.0f);

	std::deque<long double> past_net_worths;

	sf::Font options_font;

	sf::Text indicators[5];
	sf::RectangleShape heat[3];
	sf::Sprite icons[5];
	sf::Texture icons_texture[5];

	sf::Text options[8];
	sf::RectangleShape store_box[5];
	sf::Text store_box_text[5];

	double buying_rate;
	int selection = -1;
	int last_second = -1;

	int starting_index = 0;
	store * current_selection;

	store * active_store;
	user_profile * current_user;

	void process_sales();

public:
	finance(state_manager * game_ptr);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);

	void setup();
	void setup_options();
	void update_indicators();
	void setup_icons();
	void control_icon_animations(sf::Vector2f mouse_pos);
	bool handle_icons(sf::Vector2f mouse_pos);

	void add_profits(long double n_worth);
	void update_profits();

	void move_list_down();
	void move_list_up();

	void update_list();
	void update_properties();

	std::string get_average_rev();
	std::string style(long double d);
};

/*------------------------------ Inventory buy ------------------------------*/

class inventory_buy : public game_state
{
private:
	std::list<instrument *> purchaseable;

	sf::Font font;
	sf::Text title;
	sf::Text back;
	sf::Text buy_selection;
	sf::Text active_properties[4]; // value, quality, brand, type (if piano)
	sf::Text currently_showing[5];

	sf::RectangleShape details;
	
	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	instrument * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_purchaseable();
	void update_list();
	void update_properties();

public:
	inventory_buy(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
};


/*------------------------------ store_state ------------------------------*/

class store_state : public game_state
{
private:
	sf::Image in_game_printscr;

	sf::Font font;
	sf::Text title;
	sf::Text buy;
	sf::Text back;
	sf::Text set_active_store;
	sf::Text active_properties[5]; // value, area, population, max stock, BR
	sf::Text currently_showing[5];

	std::string active_store_str;

	sf::RectangleShape details;

	sf::Sprite scroll[2];
	sf::Texture scroll_texture[2];

	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	int starting_index = 0;
	store * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_icons();
	void update_list();
	void update_properties();

public:
	store_state(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	void move_list_down();
	void move_list_up();
};

/*------------------------------ store_buy ------------------------------*/

class store_buy : public game_state {
private:
	sf::Image in_game_printscr;

	std::list<store *> purchaseables;

	sf::Font font;
	sf::Text title;
	sf::Text buy;
	sf::Text back;
	sf::Text active_properties[4]; // value, area, population, max stock
	sf::Text currently_showing[5];

	std::string active_store_str;

	sf::RectangleShape details;

	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	int starting_index = 0;
	store * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_purchaseables();
	void update_list();
	void update_properties();

public:
	store_buy(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
};

/*------------------------------ staff ------------------------------*/

class staff : public game_state
{
private:
	sf::Image in_game_printscr;

	sf::Font font;
	sf::Text title;
	sf::Text buy;
	sf::Text back;
	sf::Text fire_employee;
	sf::Text active_properties[2]; // salary, skill
	sf::Text currently_showing[5];

	std::string active_employee_str;

	sf::RectangleShape details;

	sf::Sprite scroll[2];
	sf::Texture scroll_texture[2];

	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	int starting_index = 0;
	employee * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_icons();
	void update_list();
	void update_properties();

public:
	staff(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	void move_list_down();
	void move_list_up();
};

/*------------------------------ staff_hire ------------------------------*/

class staff_hire : public game_state
{
private:
	std::vector<employee *> purchaseable;

	sf::Font font;
	sf::Text title;
	sf::Text back;
	sf::Text hire_selection;
	sf::Text active_properties[2]; // salary ,skill
	sf::Text currently_showing[5];

	sf::RectangleShape details;

	sf::Sprite background;
	sf::Texture backgroud_texture;

	double buying_rate;
	int selection = -1;
	employee * current_selection;

	store * active_store;
	user_profile * current_user;

	void setup();
	void setup_text();
	void setup_purchaseable();
	void update_list();
	void update_properties();

public:
	staff_hire(state_manager * game_ptr, sf::Image print);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
};

