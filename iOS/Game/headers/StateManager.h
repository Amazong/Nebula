#pragma once

#include <stack>
#include "SFML\Graphics.hpp"
#include "GameMechanics.h"

class game_state; //so state_manager knows this exists

class state_manager
{
private:
	user_profile * current_user = nullptr;

public:
	sf::RenderWindow window;

	std::stack<game_state *> states_stack;
	
	state_manager();
	~state_manager();

	game_state * peek_state();

	void set_current_user(user_profile *user);
	user_profile * get_current_user();
	
	void push_state(game_state * state);
	int pop_state();
	void change_state(game_state * state);
	int game_loop();
	
};