#pragma once
#include <iostream>
#include <Windows.h> // to hide console
#include "SFML\Graphics.hpp"
#include "headers\GameWindows.h"
#include "headers\Music.h"
#include "headers\Errors.h"
#include "headers\StateManager.h"
#include "headers\GameStates.h"
#include "headers\Crypto.h"
#include "headers\GameMechanics.h"

int main() {
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	// uncomment line above to show console
	

	// while debugging, I'll leave the splash screen commented
	/*if (render_splash() == 42) {
		return error::file_access();
	}*/
	/*
	{
	// TESTING CRYPTO

	sf::Time elapsed;

	CryptoKey k("HelloWorld"); // include repeating mode

	CryptoFile f("LoremIpsum.txt", statuses::plaintext);
	
	std::cout << "Working..." << std::endl;
	sf::Clock t1;
	if (!(f.encrypt("encrypted", k) == 0)) return -1;
	elapsed = t1.getElapsedTime();
	std::cout << "Done!\n" << elapsed.asMicroseconds();
	std::cin.get();

	f.set_file("encrypted");
	f.set_status(statuses::encrypted);
	std::cout << "Working..." << std::endl;
	t1.restart();	
	if (!(f.decrypt("meow.txt", k) == 0)) return -1;
	elapsed = t1.getElapsedTime();
	std::cout << "Done!\n" << elapsed.asMicroseconds();
	std::cin.get();
	}
	*/

	LOGGER::log("Program started");

	MUSIC::set_m_player();

	std::thread * music_player = nullptr;
	music_player = new std::thread(&Music::loop, MUSIC::get_m_player());
	LOGGER::log("Music started playing");

	//MUSIC::get_m_player()->set_stop(true);

	user_profile ACTIVE_USER("Lady Stark");
	
	ACTIVE_USER.set_difficulty(1);
	
	ACTIVE_USER.set_net_worth(2000000);

	ACTIVE_USER.buy_store(new store(&ACTIVE_USER, "Downtown", 20000, 2));
	ACTIVE_USER.set_active_store(ACTIVE_USER.get_back_store());
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(200, "Larrivee"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(piano_brands::Steinway, piano_type::Grand, quality::Great));
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(400, "Martin"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(""));
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(600, "Maton"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(""));
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(200, "Larrivee"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(piano_brands::Steinway, piano_type::Grand, quality::Great));
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(400, "Martin"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(""));
	ACTIVE_USER.get_active_store()->buy_guitar(new guitar(600, "Maton"));
	ACTIVE_USER.get_active_store()->buy_piano(new piano(""));
	ACTIVE_USER.get_active_store()->hire_employee(new employee("Cersei", 200000, 3));
	ACTIVE_USER.get_active_store()->hire_employee(new employee("Ned Stark", 5000, 1));
	ACTIVE_USER.get_active_store()->hire_employee(new employee("Jon Snow", 100000, 2));
	ACTIVE_USER.get_active_store()->hire_employee(new employee("Sansa", 1000, 3));
	
	ACTIVE_USER.save_game();	

	std::list<instrument *> * handler = ACTIVE_USER.get_active_store()->get_inventory();
	handler->front()->set_price(1000);
	handler->back()->set_price(50000);

	ACTIVE_USER.get_active_store()->sell_algorithm();

	/* ----------------------------------------------------------
	std::string alala("Hello Game World, this is iOS.");

	show_textbox(alala, 30, 16);
	---------------------------------------------------------- */
		
	state_manager game;
	//STATE::manager = &game;

	game.push_state(new main_menu(&game)); // startup state

	return game.game_loop();
}