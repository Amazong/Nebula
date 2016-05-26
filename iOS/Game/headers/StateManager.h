#pragma once


#include <stack>
#include "SFML\Graphics.hpp"

class game_state; 

class state_manager
{
public:
	sf::RenderWindow window;


	std::stack<game_state *> states_stack;



	state_manager();
	~state_manager();

	game_state * peek_state();

	void push_state(game_state * state);
	void pop_state();
	void change_state(game_state * state);
	void game_loop();
	

};