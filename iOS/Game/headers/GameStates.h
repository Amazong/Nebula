#pragma once
#include <string>
#include "StateManager.h"
#include "SFML\Graphics.hpp"

class game_state  // base class common to all derived states.
{
private:
	bool ok = true; // by default, everything is fine

public:

	state_manager *game;

	bool is_ok() { return ok; }

	virtual void input() = 0;
	virtual void logic_update(const float elapsed) = 0;
	virtual void draw(const float elapsed) = 0;

	friend class main_menu;
	friend class options_menu;
};

/*------------------------------Main Menu------------------------------*/

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

/*------------------------------Options Menu------------------------------*/

class options_menu : public game_state
{
private:
	const std::string options_str[5] = { "Back", "Music", "Difficulty", "Framerate", "Fullscreen" };
	sf::Font options_font;
	sf::Text options[5];
	sf::Texture background;
	sf::Sprite background_sprite;
	sf::RectangleShape selector;
	int selection = -1; //by default nothing is selected

public:
	options_menu(state_manager * game_ptr);
	
	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	
	void setup_text();
};