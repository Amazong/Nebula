#pragma once

#include "SFML\Graphics.hpp"

#include <string>
#include <list>


//result of buying formula should be(for now) a flat rate (int) of chosen guitars to be sold per unit of time (to be defined)

class guitar
{
private:
	enum perceived_value { unattainable, overpriced, high, neutral, cheap, irresistible } purchasing_power; //price 0, .....  infinite , factors in chosen guitar to be sold, calculated from relation between price and value
	std::string brand;
	double value; //wholesale cost
	double price; // set by player

public:

	guitar(double value, std::string brand); // sets a value and a brand
	~guitar();

	void set_perceived_value(double relation);

	void set_price(double price);

};


class employee
{
private:
enum efficiency { low, neutral, high } skill;  //effect on buying formula
std::string name;
float salary;

public:

};


class store
{
private:

std::list<guitar *> inventory;
std::list<employee *> staff;
enum area { poor, middle, rich } setting; //effect on buying formula

std::string name;

int reputation = 0; // by default
int max_stock;
int value;
int traffic;
int buying_rate; // each store has it buying rate

public:
store();
~store();

void push_guitar(guitar * guitar);
void push_employee(employee * employee);
void pop_employee(std::string name);
void pop_guitar(); // to be expecified.

};


class user_profile
{
private:

std::list<store *> stores;
sf::Time time_elapsed = sf::seconds(0.0f);  // by omission time offset from 0 is 0. Used to calculate date (in game)
float weekly_expenses;
float net_worth;
float reputation;

public:

user_profile();
~user_profile();


void push_store(store * store);
void load_game(std::string profile_title); // pass an object of save game instead of string 
void save_game();


};

