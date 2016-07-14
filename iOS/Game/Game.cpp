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
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	// uncomment line above to show console

	// while debugging, I'll leave the splash screen commented
	/*if (render_splash() == 42) {
		return ErrNo::file_access;
	}*/
	
	LOGGER::log("Program started");

	MUSIC::set_m_player();

	std::thread * music_player = nullptr;
	music_player = new std::thread(&Music::loop, MUSIC::get_m_player());
	LOGGER::log("Music started playing");
	
	state_manager game;

	game.push_state(new main_menu(&game)); // startup state

	return game.game_loop();
}