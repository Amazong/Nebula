#pragma once

#include "SFML\Graphics.hpp"
#include <random>
#include <string>
#include <list>


// result of buying formula should be (for now) a flat rate (int) of chosen guitars to be sold per unit of time (to be defined)


/*------------------------------ instrument ------------------------------*/


class instrument
{
protected:
	enum perceived_value { unattainable, overpriced, high, neutral, cheap, irresistible } purchasing_power; // price 0, .....  infinite
																											// factors in guitar chosen to be sold, calculated from ratio between price and value
	std::string brand;
	double value; // wholesale cost
	double price; // set by player

public:
	virtual void set_perceived_value(double relation) = 0;

	virtual void set_price(double price) = 0;

	//friends
	friend class store;
};


/*------------------------------ guitar ------------------------------*/


class guitar: public instrument
{

public:

	guitar(double value, std::string & brand); // sets a value and a brand
	~guitar();

	void set_perceived_value(double relation);

	void set_price(double price);

};


/*------------------------------ employee ------------------------------*/


class employee
{

private:
	enum efficiency { low, neutral, high } skill;  //effect on buying formula
	std::string name;
	double salary;

public:
	employee(std::string & person, double value, int  num); // num: 0-low; 1-neutral; 2-high
	~employee();
	
	friend class store;
};



/*------------------------------ store ------------------------------*/

class user_profile; // so it knows this exists

class store
{
private:

	std::list<instrument *> inventory;
	std::list<employee *> staff;
	enum area { poor, middle, rich } setting; // effect on buying formula

	std::string name;

	//random  seed Generation
	std::random_device rd;
	
	user_profile * user; // so we can acess user's attributes

	int reputation = 0; // by default
	int max_stock;
	int value;
	int traffic;
	int buying_rate; // each store has it's buying rate

public:
	store(user_profile * current, std::string & name, int num); // num: 0-poor; 1-middle; 2-rich 
	~store();

//inventory management

	void buy_guitar(guitar * guitar); // needs access to user's attributes
	//to add buy piano

	void sell_algorithm();
	void sell_instrument(int position_offset);

//staff management

	void hire_employee(employee * employee);
	void fire_employee(std::string name);
	

	//friends
	friend class user_profile;
};


/*------------------------------ user_profile ------------------------------*/


class user_profile
{
private:

	std::list<store *> stores;
	sf::Time time_elapsed = sf::seconds(0.0f);  // by omission time offset from 0 is 0. Used to calculate date (in game)
	double weekly_expenses;
	double net_worth;
	double reputation;

public:

	user_profile();
	~user_profile();


	void buy_store(store * store);
	void load_game(std::string profile_title); // pass an object of save game instead of string 
	void save_game();

	//friends
	friend class store;

};

