#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>

enum ErrNo
{
	corrupt_profile = 10,
	file_access = 42,
	profile_not_found = 404,
	capture_screen_error = 101
};

namespace error {
	int trace_error(int code);

	void capture_screen_error();
	void corrupt_profile();
	void file_access();
	void profile_not_found();
};

class logger_file {
private:
	std::fstream file;
	std::string file_name;

public:
	logger_file();
	logger_file(std::string filename);
	~logger_file();
	
	void log(std::string);
};

namespace LOGGER {
	extern void log(std::string str);
};