#pragma once

#include "SFML\Graphics.hpp"
#include <random>
#include <string>
#include <list>
#include <fstream>
#include <cstdlib>
#include "Errors.h"
// result of buying formula should be (for now) a flat rate (int) of chosen guitars to be sold per unit of time (to be defined)



struct save_user //for saving user.
{
	char user[51];
	double weekly_expenses;
	double net_worth;
	double reputation;
	int difficulty;
};


/*------------------------------ instrument ------------------------------*/


class instrument
{
protected:
	enum perceived_value { unattainable, overpriced, high, neutral, cheap, irresistible } purchasing_power; // price 0, .....  infinite
																											// factors in guitar chosen to be sold, calculated from ratio between price and value
	char  brand[51];
	double value; // wholesale cost
	double price; // set by player

public:
	bool is_guitar;


	void set_price(double price);
	virtual void set_perceived_value(double ratio) = 0;

	// friends
	friend class store;
};


/*------------------------------ guitar ------------------------------*/


class guitar: public instrument
{

public:
	guitar() {};
	guitar(double value, char * brand); // sets a value and a brand


	void set_perceived_value(double ratio);
};

/*------------------------------ piano ------------------------------*/


class piano : public instrument
{

public:
	piano() {};
	piano(double value, char * brand); // sets a value and a brand
	
	void set_perceived_value(double ratio);
};


/*------------------------------ employee ------------------------------*/


class employee
{

private:
	enum efficiency { low, neutral, high } skill;  //effect on buying formula
	char name[51];
	double salary;

public:
	employee() {};
	employee(char * person, double value, int  num); // num: 0-low; 1-neutral; 2-high
	
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

	char name[51];

	//random  seed Generation
	std::random_device rd;
	
	user_profile * user; // so we can acess user's attributes

	double reputation = 0; // by default
	unsigned int max_stock;
	int value;
	int traffic;
	int buying_rate; // each store has it's buying rate

public:
	store() {};
	store(const store & shop); // does not copy the std::lists nor the user pointer
	store(user_profile * current, char * name, int value, int num); // num: 0-poor; 1-middle; 2-rich 
	~store();

	store & operator = (const store & store);
	// store management
	void set_reputation(double rep) { reputation = rep; };
	double get_reputation() { return reputation; };

	// inventory management
	int get_max_stock();
	int get_stock();
	void buy_guitar(guitar * guitar); // needs access to user's attributes
	void buy_piano(piano * piano); // to implement

	void sell_algorithm();
	void sell_instrument(int position_offset);

	// staff management

	void hire_employee(employee * employee);
	void fire_employee(char * name);
	
	//instrument, guitar and piano all have the same sizes
	// uses adm, memory must be freed after use of this function.
	// save inventory and staff -- returns array with elements and size in argument // inventory gives as guitars, but they will be casted in the loading function. with isguitar member
	guitar * inventory_tab(int & size);
	employee * staff_tab(int & size);

	void fill_inventory(guitar * tab, int size); // allocates its own
	void fill_staff(employee * tab, int size);	// allocates its own

	// friends
	friend class user_profile;
};


/*------------------------------ user_profile ------------------------------*/


class user_profile
{
private:
	std::list<store *> stores;
	store * active_store;
	sf::Time time_elapsed = sf::seconds(0.0f);  // by omission time offset from 0 is 0. Used to calculate date (in game)
	
	char user[51];
	double weekly_expenses;
	double net_worth;
	double reputation;
	int difficulty = -1; // difficulty: 0-easy; 1-medium; 2-hard

public:

	user_profile() {};
	user_profile(char * name);
	user_profile(const user_profile & user); // does not copy the std::lists nor active store
	~user_profile();
	
	store * get_active_store();
	void set_active_store(store * active_store_new);
	bool set_active_store(unsigned int store_id);

	int get_difficulty() { return difficulty; }
	void set_difficulty(int diff) { difficulty = diff; }

	void buy_store(store * store);

	void save_game();
	void save_inventories(std::string  user, const guitar * tab, int size, int store_index);
	void save_staff(std::string  user, const employee * tab, int size, int store_index);
	void save_stores(std::string  user, const store * tab, int size);
	void load_game(std::string  profile_title); // pass an object of save game instead of string
	void load_user(std::string & profile_title);
	void load_stores(user_profile * user);
	void load_store_inv(const user_profile * user, store & shop, int store_index);
	void load_store_staff(const user_profile * user, store & shop, int store_index);

	//friends
	friend class store;

};

