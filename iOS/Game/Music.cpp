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

	return songs[index];
}

void Music::loop()
{
	sf::Time fade = sf::seconds(2.0f);
	playing_now->setVolume(0);
	playing_now->play();
	sf::Clock clk;
	
	for (float i = 0; i < 100; i += 5) {
		clk.restart();
		playing_now->setVolume(i);
		while (clk.getElapsedTime().asSeconds() < 0.1);
	}

	playing_now->pause(); // not perceptible
	
	while (true) {
		if (!stop) {
			if (!playing_next->openFromFile("res/sound/" + get_random())) {
				error::file_access();
			}
			playing_now->play();
			while (playing_now->getPlayingOffset() < (playing_now->getDuration() - fade));
			
			if (playing_next == nullptr) return;
			else playing_next->play();
			
			while (playing_now->getStatus() == sf::SoundSource::Status::Playing);
			
			if (playing_now != nullptr) delete playing_now;
			else return;
			
			if (playing_next == nullptr) return;
			
			playing_now = playing_next;
			playing_next = new sf::Music;
		}
		if (quit) break;
	}
}

namespace MUSIC {
	Music * m_player;

	Music * set_m_player() {
		if (m_player != nullptr) m_player = new Music;
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