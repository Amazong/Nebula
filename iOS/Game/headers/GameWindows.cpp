#include "GameWindows.h"
#include "Errors.h"

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

	//edge cases

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


	//now tab_ptr[j] has all words of phrase.

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
	int size, offset = 5;
	sf::Font test_font;


	std::string *  placeholder = get_string_tab(str, size, line_size);

	if (!test_font.loadFromFile("res/fonts/Roboto-Bold.ttf"))
	{
		error::trace_error(ErrNo::file_access);
	}

	sf::Text * text = new sf::Text[size];


	sf::Vector2f window_size(((line_size * char_size) / 2.0f) + (char_size * 3.0f), (char_size * 3.0f) + (size * char_size) + ((size - 1.0f) * offset));


	sf::RenderWindow window(sf::VideoMode(unsigned int(window_size.x), unsigned int(window_size.y)), "", sf::Style::Close);

	sf::Vector2f pos(0.5f * window.getSize().x, 0.15f * window.getSize().y);

	for (int i = 0; i < size; i++, pos.y += offset)
	{
		text[i].setFont(test_font);
		text[i].setCharacterSize(char_size);
		text[i].setString(placeholder[i]);
		text[i].setColor(sf::Color::Black);
		text[i].setOrigin((text[i].getGlobalBounds().width / 2.0f), (text[i].getGlobalBounds().height / 2.0f)); // origin in center.
		text[i].setPosition(pos);

		pos.y += char_size;

	}


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Return)
					window.close();
		}


		window.clear(sf::Color::White);
		for (int i = 0; i < size; i++)	window.draw(text[i]);
		window.display();
	}

	
	(size == 1)? delete placeholder : delete[] placeholder; //if size == 1 i used new, not new[]
	delete[] text;
}