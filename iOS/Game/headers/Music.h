#pragma once
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "Errors.h"
#include "SFML\Audio.hpp"
#include "SFML\System.hpp"

class Music
{
private:
	sf::Time fade = sf::seconds(2.0f);

	bool stop = false;
	bool quit = false;
	bool skip = false;

	std::string songs[14] = { "BackgroundProgression.ogg", "Bossa1.5.ogg",
		"Bossa2.ogg", "BossaBossa.ogg", "BossaBossaBossa.ogg", "Butterfly.ogg",
		"Forever Young.ogg", "HotaruNoShikai.ogg", "InMyLife.ogg", "Kygo.ogg", 
		"OpenGJam.ogg", "Paradise.ogg",	"SweetChildOMine.ogg", "Tears in Heaven.ogg" };
	std::string playing_now_str; // for debugging purposes
	sf::Music *playing_now;
	sf::Music *playing_next;

	std::string get_random();

public:
	Music();
	~Music();

	bool get_stop() { return stop; }
	bool get_quit() { return quit; }
	bool get_skip() { return skip; }
	void set_skip(bool setter) { skip = setter; }
	void set_stop(bool setter) { stop = setter; }
	void set_quit(bool setter) { quit = setter; }
	void loop();
};

namespace MUSIC {
	extern Music * set_m_player();
	extern Music * get_m_player();
}