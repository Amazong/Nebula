#pragma once

#include "GameStates.h"
#include <stack>
#include "SFML\Graphics.hpp"


class state_manager
{
public:

	std::stack<game_state *> states_stack;
	sf::RenderWindow window;


	state_manager();
	~state_manager();

	game_state * peek_state();

	void push_state(game_state * state);
	void pop_state();
	void change_state(game_state * state);
	void game_loop();
	

};