#pragma once

#include "SFML\Graphics.hpp"
#include <random>
#include <string>
#include <list>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include "Errors.h"

struct save_user //for saving user.
{
	char user[51];
	double weekly_expenses;
	double net_worth;
	double reputation;
	int difficulty;
	float time_elapsed;
};

/*------------------------------ instrument ------------------------------*/

namespace piano_brands {
	const enum piano_brands { NA, Roland, Korg, Kawai, Yamaha, Steinway };
}
namespace piano_type {
	const enum piano_type { NA, Digital, Upright, Grand };
}
namespace quality {
	const enum quality { NA, Poor, Fair, Great };
}


class instrument
{
protected:
	enum perceived_value { unattainable, overpriced, high, neutral, cheap, irresistible } purchasing_power; // price 0, ..... infinite
																											// factors in guitar chosen to be sold, calculated from ratio between price and value
	char brand[51] = "NA";
	char piano_brands[5][51] = { "Roland", "Korg", "Kawai", "Yamaha", "Steinway & Sons" };
	double value; // wholesale cost
	double price; // set by player

	piano_brands::piano_brands own_brand_piano;
	piano_type::piano_type own_type_piano;
	quality::quality own_quality;

public:
	bool is_guitar;

	void set_price(double price);
	double get_price() { return price; }
	double get_value() { return ((int)(value*100))/100.0; } // only two decimal places
	virtual void set_perceived_value(double ratio) = 0;

	perceived_value get_perceived_value() { return purchasing_power; }

	piano_brands::piano_brands get_brand_piano() { return own_brand_piano; }
	piano_type::piano_type get_type_piano() { return own_type_piano; }
	quality::quality get_quality() { return own_quality; }

	// std::string returns
	char * print_brand() { return brand; }
	std::string print_brand_cpp();
	std::string print_brand_cpp_short();
	std::string print_type_cpp();
	std::string get_value_cpp();
	std::string get_price_cpp();
	std::string style(double d = 0);
	std::string get_quality_cpp();
	std::string get_perceived_value_cpp();

	// friends
	friend class store;
};


/*------------------------------ guitar ------------------------------*/


class guitar : public instrument
{
public:
	guitar() {};
	guitar(double value, char * brand); // sets a value and a brand
	guitar(std::string name);
	guitar(piano_brands::piano_brands brand, piano_type::piano_type type, quality::quality quality); // invalid, for guitars

	void set_perceived_value(double ratio);
};

/*------------------------------ piano ------------------------------*/


class piano : public instrument
{
public:
	piano() {};
	piano(double value, char * brand);
	piano(std::string name);
	piano(piano_brands::piano_brands brand,
		piano_type::piano_type type,
		quality::quality quality); // sets a value and a brand

	void set_perceived_value(double ratio);
};


/*------------------------------ employee ------------------------------*/


class employee
{
private:
	enum efficiency { low = 1, neutral, high } skill; //effect on buying formula
	char name[51];
	double salary;

public:
	employee() {};
	employee(char * name);
	employee(char * person, double value, int eff); // eff: 1-low; 2-neutral; 3-high

	// string returns
	std::string get_name() { std::string s = name; return s; }
	std::string get_salary();

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
	enum population { rural, town, city } placement;

	char name[51];

	//random seed Generation
	std::random_device rd;

	user_profile * user; // so we can acess user's attributes

	double reputation = 0; // by default
	double average_purchasing_power;
	double average_efficiency;
	
	unsigned int max_stock;
	
	double value;
	int traffic;
	int buying_rate; // each store has it's buying rate

public:
	store() {};
	store(const store & shop); // does not copy the std::lists nor the user pointer
	store(user_profile * current, char * name, double value, int areacode, int pop = 1); // areacode: 0-poor; 1-middle; 2-rich
	store(user_profile * current, char * name);
	~store();

	store & operator = (const store & store);
	
	// store management
	void set_reputation(double rep) { reputation = rep; };
	double get_reputation() { return reputation; };
	
	// averages
	void update_average_purchasing_power();
	void update_average_efficiency();
	void update_averages() { update_average_purchasing_power(); update_average_efficiency(); }
	double get_average_purchasing_power() { return average_purchasing_power; };
	double get_average_efficiency() { return average_efficiency; };
	bool run_probability(double prob);
	void update_traffic();

	// inventory management
	int get_max_stock();
	int get_stock();
	std::list<instrument *> * get_inventory() { return &inventory; }
	void buy_guitar(guitar * guitar); // needs access to user's attributes
	void buy_piano(piano * piano); // to implement

	void sell_algorithm();
	void sell_instrument(int position_offset);

	// staff management
	void hire_employee(employee * employee);
	void fire_employee(char * name);

	// instrument, guitar and piano all have the same sizes
	// uses adm, memory must be freed after use of this function.
	// save inventory and staff -- returns array with elements and size in argument // inventory gives as guitars, but they will be casted in the loading function. with isguitar member
	guitar * inventory_tab(int & size);
	employee * staff_tab(int & size);

	void fill_inventory(instrument * tab, int size); // allocates its own
	void fill_staff(employee * tab, int size);	// allocates its own

	// string returns
	std::string get_name_cpp() { std::string s = name; return s; };
	std::string get_value_cpp();

	// friends
	friend class user_profile;
	friend class in_game_setup;
};


/*------------------------------ user_profile ------------------------------*/

class user_profile
{
private:
	std::list<store *> stores;
	store * active_store = nullptr;
	sf::Time time_elapsed = sf::seconds(0.0f); // by omission time offset from 0 is 0. Used to calculate date (in game)
	
	char user[51];
	double weekly_expenses;
	long double net_worth;
	double reputation;
	int difficulty = -1; // difficulty: 0-easy; 1-medium; 2-hard

public:
	const float WEEK_TIME_SECS = 2.0f;
	const float YEAR_TIME_SECS = WEEK_TIME_SECS * 52.0f;

	user_profile() {};
	user_profile(char * name);
	user_profile(const user_profile & user); // does not copy the std::lists nor active store
	~user_profile();

	void set_net_worth(double worth) { net_worth = worth; }

	store * get_active_store();
	bool set_active_store(store * active_store_new);
	bool set_active_store(unsigned int store_id);

	int get_difficulty() { return difficulty; }
	void set_difficulty(int diff) { difficulty = diff; }

	void buy_store(store * store);
	store * get_back_store() { return stores.back(); }

	void set_user_name(std::string s) { strcpy_s(this->user, s.c_str()); }

	std::string get_time_str() const;

	double get_reputation() { return reputation; }

	void save_game();
	void save_inventories(std::string user, const guitar * tab, int size, int store_index);
	void save_staff(std::string user, const employee * tab, int size, int store_index);
	void save_stores(std::string user, const store * tab, int size);
	void load_game(std::string profile_title); // pass an object of save game instead of string
	void load_user(std::string & profile_title);
	void load_stores(user_profile * user);
	void load_store_inv(const user_profile * user, store & shop, int store_index);
	void load_store_staff(const user_profile * user, store & shop, int store_index);

	// string returns
	std::string get_name_cpp() const { std::string s = user; return s; }
	std::string get_balance_styled(int arg = 0) const;

	// friends
	friend class store;
	friend class in_game;
	friend class in_game_setup;
};

