#pragma once
#include <string>
#include <random>
#include <thread>
#include "Errors.h"
#include "SFML\Audio.hpp"
#include "SFML\System.hpp"

class Music
{
private:
	bool stop = false;
	bool quit = false;

	std::string songs[14] = { "BackgroundProgression.ogg", "Bossa1.5.ogg",
		"Bossa2.ogg", "BossaBossa.ogg", "BossaBossaBossa.ogg", "Butterfly.ogg",
		"Forever Young.ogg", "HotaruNoShikai.ogg", "InMyLife.ogg", "Kygo.ogg", 
		"OpenGJam.ogg", "Paradise.ogg",	"SweetChildOMine.ogg", "Tears in Heaven.ogg" };
	sf::Music *playing_now;
	sf::Music *playing_next;

	std::string get_random();

public:
	Music();
	~Music();

	void set_stop(bool setter) { stop = setter; }
	void set_quit(bool setter) { quit = setter; }
	void loop();
};

namespace MUSIC {
	extern Music * set_m_player();
	extern Music * get_m_player();
}