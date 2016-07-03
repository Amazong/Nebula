#include <iostream>
//#include <Windows.h> // to hide console; to uncomment on final version, together with code below
#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Errors.h"
#include "headers\StateManager.h"
#include "headers\GameStates.h"
#include "headers\Crypto.h"

int main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// uncomment line above to show console
	

	// while debugging, I'll leave the splash screen commented
	/*if (render_splash() == 42) {
		return error::file_access();
	}*/

	// TESTING CRYPTO
	
	CryptoKey k("LoremIpsum"); // include repeating mode

	CryptoFile f("meow.txt", statuses::plaintext);
	if (!f.encrypt("encrypted", k)) return -1;
	std::cin.get();
	if (!f.decrypt("meow.txt", k)) return -1;
	std::cin.get();

	return 0;

	state_manager game;

	game.push_state(new main_menu(&game)); // startup state
	
	return game.game_loop();
}