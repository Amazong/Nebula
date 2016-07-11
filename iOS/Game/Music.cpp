#include "headers\Music.h"

Music::Music()
{
	playing_now = new sf::Music;
	playing_next = new sf::Music;
	if (!playing_now->openFromFile("res/sound/" + get_random())) {
		error::file_access();
	}
}

Music::~Music()
{
	if (playing_now != nullptr) delete playing_now; 
	if (playing_next != nullptr) delete playing_next;
}

std::string Music::get_random()
{
	std::random_device rd;
	std::mt19937 random(rd());
	std::uniform_int_distribution<int> range(0, 13);

	int index = range(random);

	playing_now_str = songs[index];

	return songs[index];
}

void Music::set_MAX_VOL(float v)
{
	if (v >= 0 && v <= 100) {
		MAX_VOL = v;
		playing_now->setVolume(v);
		playing_next->setVolume(v);
	}
}

void Music::loop()
{
	playing_now->setVolume(0);
	playing_now->play();
	LOGGER::log("Started playing " + playing_now_str);
	sf::Clock clk;
	
	for (float i = 0; i <= MAX_VOL; i += 5) {
		clk.restart();
		playing_now->setVolume(i);
		while (clk.getElapsedTime().asSeconds() < 0.1);
	}
	
	while (true) {
		if (quit) {
			for (float i = MAX_VOL; i >= 0; i -= 5) {
				clk.restart();
				playing_now->setVolume(i);
				while (clk.getElapsedTime().asSeconds() < 0.05);
			}

			LOGGER::log("Quitting music player");
			break;
		}

		if (!stop) {
			if (!playing_next->openFromFile("res/sound/" + get_random())) {
				error::file_access();
			}
			
			while (playing_now->getPlayingOffset() < (playing_now->getDuration() - fade)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200)); // more CPU friendly
				if (stop) {
					for (float i = MAX_VOL; i >= 0; i -= 5) {
						clk.restart();
						playing_now->setVolume(i);
						while (clk.getElapsedTime().asSeconds() < 0.1);
					}
					playing_now->stop();
					LOGGER::log("Stopping playback");
					break;
				}

				if (skip || quit) {
					break;
				}
			}

			if (stop || quit) continue;
			
			playing_next->setVolume(0);
			playing_next->play();
			LOGGER::log("Started playing " + playing_now_str);

			for (float i = 0; i <= MAX_VOL; i += 5) { // crossfade
				clk.restart();
				playing_now->setVolume(MAX_VOL - i);
				playing_next->setVolume(i);
				while (clk.getElapsedTime().asSeconds() < 0.1);
			}

			if (!skip) { // wait for song to finish
				while (playing_now->getStatus() == sf::SoundSource::Status::Playing) {
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
			}
			
			delete playing_now;
			
			playing_now = playing_next;
			playing_next = new sf::Music;
		}
		
		if (skip) {
			skip = false;
			LOGGER::log("Skipping song");
			continue;
		}

		if (!stop) continue;

		while (stop) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		if (skip) { // accounts for skipping while stopped
			delete playing_now;
			playing_now = playing_next;
			skip = false;
			playing_next = new sf::Music;
		}

		// fade in for resuming after stopping
		playing_now->setVolume(0);
		playing_now->play();
		for (float i = 0; i <= MAX_VOL; i += 5) {
			clk.restart();
			playing_now->setVolume(i);
			while (clk.getElapsedTime().asSeconds() < 0.1);
		}
	}
}

namespace MUSIC {
	Music * m_player = nullptr;

	Music * set_m_player() {
		if (m_player == nullptr) m_player = new Music;
		else {
			delete m_player;
			m_player = new Music;
		}

		return m_player;
	}

	Music * get_m_player() {
		return m_player;
	}
}