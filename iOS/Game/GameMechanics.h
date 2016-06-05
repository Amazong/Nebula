#pragma once

#include "SFML\Graphics.hpp"

#include <string>
#include <list>


class guitars
{



};


class employees
{


};


class stores
{
private:
	std::list<guitars *> inventory;
	std::list<employees *> staff;


public:


};


class user_profile
{
private:

	std::list<stores *> stores;
	sf::Time time_elapsed = sf::seconds(0.0f);  // by omission time offset from 0 is 0.
	float weekly_expenses;
	float net_worth;
	float reputation;

public:
	





};

