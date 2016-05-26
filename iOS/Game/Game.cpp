#include <iostream>
#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Errors.h"
#include "headers\StateManager.h"
#include "headers\GameStates.h"

int main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// Descomentar a linha acima para esconder a consola
	

	if (render_splash() == 42) {
		error::file_access();
	}

	
	state_manager game;

	game.push_state(new main_menu(&game)); // startup state
	game.game_loop();

	
	return(0);
}