#pragma once
#include "StateManager.h"
#include "SFML\Graphics.hpp"

class game_state  //base class common to all derived states.
{
private:
	bool ok = true;

public:

	state_manager *game;

	bool is_ok() { return ok; }

	virtual void input() = 0;
	virtual void logic_update(const float elapsed) = 0;
	virtual void draw(const float elapsed) = 0;

	friend class main_menu;
};


class main_menu : public game_state
{
private:
	sf::Texture background;
	sf::Sprite menu_background_sprite;

public:

	main_menu(state_manager * game);

	void input();
	void logic_update(const float elapsed);
	void draw(const float elapsed);
	int load_texture();
};