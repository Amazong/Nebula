#pragma once

#include "SFML\Graphics.hpp"

#include <cstring>
#include <list>


//result of buying formula should be(for now) a flat rate (int) of chosen guitars to be sold per unit of time (to be defined)

class guitars
{
private:
	enum perceived_value{ unattainable, overpriced, high , neutral, cheap, irresistible } purchasing_power; //price 0, .....  infinite , factors in chosen guitar to be sold, calculated from relation between value and price
	float value; //wholesale cost
	float price; // set by player

public:


};


class employees
{
private:
	enum efficiency { low, neutral, high } skill;  //effect on buying formula
	char name[30];
	float salary;

public:
	
};


class stores
{
private:

	std::list<guitars *> inventory;
	std::list<employees *> staff;
	int reputation = 0; // by default
	int max_stock;
	int value;
	int traffic;
	enum area { poor, middle, rich } setting; //effect on buying formula

	
public:


};


class user_profile
{
private:

	std::list<stores *> stores;
	sf::Time time_elapsed = sf::seconds(0.0f);  // by omission time offset from 0 is 0. Used to calculate date (in game) 
	float weekly_expenses;
	float net_worth;
	float reputation;

public:
	





};

