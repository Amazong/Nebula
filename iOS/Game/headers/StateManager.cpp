#include <stack>
#include "SFML\Graphics.hpp"

#include "StateManager.h"
#include "GameStates.h"

state_manager::state_manager()
{
	window.create(sf::VideoMode::getDesktopMode(), "iOS", sf::Style::Fullscreen);
	window.setFramerateLimit(60);  // default framerate
}

state_manager::~state_manager()
{
	while (!states_stack.empty())
	{
		pop_state();
	}
}

game_state * state_manager::peek_state() //does not pop
{
	if(states_stack.empty())
		return nullptr;
	else
	{
		return(states_stack.top());
	}
}

void state_manager::push_state(game_state * state)
{
	states_stack.push(state);
}

void state_manager::pop_state()
{
	delete states_stack.top();
	states_stack.pop();
}

void state_manager::change_state(game_state * state)
{
	if (!states_stack.empty())
		pop_state();
	push_state(state);

}

void state_manager::game_loop()
{
	sf::Clock clk;

	while (window.isOpen())
	{
		if (peek_state() == nullptr) continue;

		sf::Time elapsed = clk.restart();

		peek_state()->input();
		peek_state()->logic_update(elapsed.asSeconds());
		window.clear();
		peek_state()->draw(elapsed.asSeconds());
		window.display();
	}
}
