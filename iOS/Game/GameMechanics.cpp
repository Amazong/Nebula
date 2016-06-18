#include "headers\GameMechanics.h"


/*------------------------------ guitar ------------------------------*/

guitar::guitar(double value, std::string & name)
{
	this->price = this->value = value;
	this->brand = name;

	set_perceived_value(this->price / this->value);

 }

guitar::~guitar()
{
}

void guitar::set_perceived_value(double relation)
{
	if (relation <= 0.1 )
		purchasing_power = perceived_value::irresistible;
	else if (relation <= 1.5 && relation > 0.1 )
		purchasing_power = perceived_value::cheap;
	else if (relation <= 3 && relation > 1.5 )
		purchasing_power = perceived_value::neutral;
	else if (relation <= 5 && relation > 3 )
		purchasing_power = perceived_value::high;
	else if (relation <= 10 && relation > 5 )
		purchasing_power = perceived_value::overpriced;
	else if (relation > 10)
		purchasing_power = perceived_value::unattainable;
}

void guitar::set_price(double price)
{
	this->price = price;
	set_perceived_value(this->price / this->value);
}


/*------------------------------ employee ------------------------------*/


employee::employee(std::string & person, double value, int num) // num: 0-low; 1-neutral; 2-high
{
	this->name = person;
	salary = value;
	this->skill = efficiency(num);
}

employee::~employee()
{
}


/*------------------------------ store ------------------------------*/


store::store(user_profile * current, std::string & name, int num) // num: 0-poor; 1-middle; 2-rich 
{
	this->user = current;
	this->name = name;
	this->setting = static_cast<area>(num);
	this->max_stock = 50 + 10 * setting; // can be altered, formula for max inventory
}

store::~store()
{
		//to work on
}

void store::buy_guitar(guitar * guitar)
{
	if (user->net_worth >= guitar->value && inventory.size() < max_stock) // buying from wholesale
	{
		user->net_worth -= guitar->value;
		this->inventory.push_back(guitar); 
		return;
	}
	
	// else we need to deploy a message error (warning window)
}

void store::sell_algorithm()
{
	if (!inventory.empty()) // only when inventory is not empty
	{
		std::mt19937 random_numbers(rd()); // random number generator algorithm
		int position;

		for ( int i = buying_rate ; i != 0; i-- )
		{
			std::uniform_int_distribution<int> inventory_range(0, (inventory.size() - 1)); // random position to eliminate

			position = inventory_range(random_numbers); // position 0 -> size-1

			sell_instrument(position); // to be redefined upon piano class deployment


		}
	}

	// deploy message your inventory is empty. (else)
}

void store::sell_instrument(int position_offset)
{
	std::list<instrument *>::iterator it = inventory.begin();

	for ( ; position_offset != 0; position_offset--)  // proceding to position offset
	{
		it++;
	}

	// *it -> pointer to guitar
	user->net_worth += (*it)->price;
	this->reputation += ((*it)->value * 0.05); // 5% of inventory's value added to store reputation

	delete (*it); // memory management

	inventory.erase(it);
	
}

void store::hire_employee(employee * employee)
{
	staff.push_back(employee); 
}

void store::fire_employee(std::string name)
{
	std::list<employee *>::iterator it;

	for ( it = staff.begin() ; it != staff.end(); it++)
	{
		if ((*it)->name.compare(name) == 0)
		{
			delete (*it);
			staff.erase(it);
			return;
		}

	}

	// person not found
}


/*------------------------------ user_profile ------------------------------*/


user_profile::user_profile()
{

}

user_profile::~user_profile()
{

}

void user_profile::buy_store(store * store)
{
	if ( this->net_worth >= store->value)
	{
		this->net_worth -= store->value;
		stores.push_back(store);
	}
}
