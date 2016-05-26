#pragma once
#include "StateManager.h"
#include "SFML\Graphics.hpp"

class game_state  //base class common to all derived states.
{

public:

	state_manager *game;

	virtual void input() = 0;
	virtual void logic_update(const float elapsed) = 0;
	virtual void draw(const float elapsed) = 0;



};


class main_menu : public game_state
{
private:

	sf::Texture background;

public:

	main_menu(state_manager * game);
	
	virtual void input();
	virtual void logic_update(const float elapsed);
	virtual void draw(const float elapsed);

};