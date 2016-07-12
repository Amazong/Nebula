#include <stack>
#include "SFML\Graphics.hpp"
#include "headers\StateManager.h"
#include "headers\GameStates.h"
#include "headers\Errors.h"

state_manager::state_manager()
{
	window.create(sf::VideoMode::getDesktopMode(), "iOS", sf::Style::Fullscreen);
	//window.create(sf::VideoMode(800,600), "iOS"); // for debugging
	window.setFramerateLimit(60);  // default framerate
}

state_manager::~state_manager()
{
	delete current_user;
	
	while (!states_stack.empty())
	{
		pop_state();
	}
}

game_state * state_manager::peek_state() // does not pop
{
	if(states_stack.empty())
		return nullptr;
	else
	{
		return(states_stack.top());
	}
}

user_profile * state_manager::get_current_user()
{
	return current_user;
}

void state_manager::set_current_user(user_profile * user)
{
	delete current_user;
	current_user = user;
}

void state_manager::push_state(game_state * state)
{
	states_stack.push(state);
}

int state_manager::pop_state()
{
	if (!(states_stack.top()->is_ok())) {
		states_stack.top()->make_ok(); // to show that it's been handled; prevents infinite loop
		return error::trace_error(states_stack.top()->get_error_code());		
	}
	delete states_stack.top();
	states_stack.pop();
	return 0;
}

void state_manager::change_state(game_state * state)
{
	if (!states_stack.empty())
		pop_state();
	push_state(state);
}

int state_manager::game_loop()
{
	sf::Clock clk;
	sf::Time elapsed;

	while (window.isOpen())
	{
		elapsed = clk.restart();
		
		if (peek_state() == nullptr) continue;

		if (!(peek_state()->is_ok())) {
			return pop_state();
		}

		peek_state()->input();
		peek_state()->logic_update(elapsed.asSeconds());
		window.clear(sf::Color::Black);
		peek_state()->draw(elapsed.asSeconds());
		window.display();
	}

	LOGGER::log("Program exited normally");
	return 0;
}
