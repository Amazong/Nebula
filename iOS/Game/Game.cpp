#include <iostream>
//#include <Windows.h> // to hide console; to uncomment on final version, together with code below
#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Errors.h"
#include "headers\StateManager.h"
#include "headers\GameStates.h"


int main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// uncomment line above to show console
	

	// while debugging, I'll leave the splash screen commented
	/*if (render_splash() == 42) {
		return error::file_access();
	}*/


	state_manager game;

	game.push_state(new main_menu(&game)); // startup state
	if (!(game.peek_state()->is_ok())) {
		return error::file_access();
	}
	game.game_loop();
	
	return(0);
}