#include "headers\GameWindows.h"
#include "headers\Errors.h"

int render_splash()
{
	sf::RenderWindow *splash_screen = new sf::RenderWindow(sf::VideoMode(887, 407), "", sf::Style::None);
	sf::Clock splash_clk;
	
	sf::Texture splash_png;
	if (!(splash_png.loadFromFile("res/png/Splash.png"))) {
		splash_screen->setVisible(0);
		return 42;
	}
	sf::Sprite splash_sprite;
	splash_sprite.setTexture(splash_png);

	while (splash_clk.getElapsedTime().asSeconds() < 4.0f) {
		splash_screen->clear(sf::Color::Black);
		splash_screen->draw(splash_sprite);
		splash_screen->display();
	}
	
	delete splash_screen;
	return 0;
}


int n_words(const std::string & str)
{
	if (str.empty())
		return(0);

	int counter = 0;
	int size = str.length();
	int num = str.find_first_of(' ');
	if (num == std::string::npos)
		return(1);

	while (num != std::string::npos)
	{
		counter++;
		num = str.find_first_of(' ', num + 1);

	}

	return(counter + 1);
}

std::string * get_string_tab(std::string & str, int & size, unsigned int line_size)
{
	if (str.empty())
	{
		size = 1;
		return(new std::string(""));
	}
	if (str.length() <= line_size)
	{
		size = 1;
		return(new std::string(str));
	}
	int words = n_words(str), pos = 0, counter = 0, i = 0;

	size = str.length();

	if (words == 1)
	{
		size = 1;
		return(new std::string(str));
	}

	// edge cases

	std::string * tab_ptr = new std::string[words];

	for (int i = 0, j = 0; i < size; i++)
	{
		if (str[i] == ' ')
		{
			tab_ptr[j] = str.substr(pos, (i - pos)) + ' ';

			pos = ++i;
			counter++;
			j++;
		}

		if (words - counter == 1)
			tab_ptr[j] = str.substr(pos);
	}


	// now tab_ptr[j] has all words of phrase.

	std::vector<std::string *> fitted_strings;

	std::string temp = "";

	for (int i = 0; i < words; i++)
	{
		if (temp.size() + tab_ptr[i].size() > line_size)
		{
			fitted_strings.push_back(new std::string(temp));

			temp.clear();
			temp += tab_ptr[i];
			continue;
		}

		temp += tab_ptr[i];

	}
	fitted_strings.push_back(new std::string(temp));

	delete[] tab_ptr;
	size = fitted_strings.size();

	std::string * str_ptr = new std::string[size];

	for (int i = 0; i < size; i++)
	{
		str_ptr[i] = *(fitted_strings[i]);
		delete fitted_strings[i];
	}

	return(str_ptr);

}

void show_textbox(std::string & str, unsigned int line_size, unsigned int char_size)			//NO DOUBLE FUCKING SPACES
{
	
}