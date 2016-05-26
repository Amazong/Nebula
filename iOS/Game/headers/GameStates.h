#pragma once
#include "StateManager.h"


class game_state  //base class common to all derived states.
{

public:

	state_manager * game;

	virtual void draw(const float elapsed) = 0;
	virtual void logic_update(const float elapsed) = 0;
	virtual void input() = 0;

};