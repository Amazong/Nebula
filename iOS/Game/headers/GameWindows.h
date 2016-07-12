#pragma once

#include <vector>
#include "SFML\Graphics.hpp"
#include <string>

int render_splash();

std::string * get_string_tab(std::string & str, int & size, unsigned int line_size); // gives tab of preconfigured strings according to line size ( dynamic), be sure to delete[] (or delete) in the end. everything else is self contained.

int n_words(const std::string & str); // gives words in raw std::string
//void show_textbox(std::string & str, unsigned int line_size, unsigned  int char_size); // function for message boxes., line size in chars.