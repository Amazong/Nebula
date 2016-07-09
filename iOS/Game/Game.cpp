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
	
	/*{
	// TESTING CRYPTO

	CryptoKey k("LoremIpsum"); // include repeating mode

	CryptoFile f("meow.txt", statuses::plaintext);
	
	sf::Clock t1;
	std::cout << "Working..." << std::endl;
	if (!f.encrypt("encrypted", k)) return -1;
	std::cout << "Done!\n" << t1.getElapsedTime().asMicroseconds();
	std::cin.get();

	t1.restart();
	std::cout << "Working..." << std::endl;
	if (!f.decrypt("meow.txt", k)) return -1;
	std::cout << "Done!\n" << t1.getElapsedTime().asMicroseconds();
	std::cin.get();
	}*/
	
	std::string alala("Hello Game World, this is iOS.");

	show_textbox(alala, 30, 16);
	state_manager game;

	game.push_state(new main_menu(&game)); // startup state
	
	return game.game_loop();
}