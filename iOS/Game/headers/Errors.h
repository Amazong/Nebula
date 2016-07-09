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
};

namespace error {
	int trace_error(int code);

	void corrupt_profile();
	void file_access();
	void profile_not_found();
}

class logger {
private:
	std::fstream file;
	std::string file_name;

public:
	logger();
	logger(std::string filename);
	~logger();
	
	void log(std::string);
};