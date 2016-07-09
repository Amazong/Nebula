#include "headers\GameMechanics.h"


/*------------------------------ instrument ------------------------------*/

void instrument::set_price(double price)
{
	this->price = price;
	set_perceived_value(this->price / this->value);
}

/*------------------------------ guitar ------------------------------*/

guitar::guitar(double value, char * name)
{
	is_guitar = true;
	this->price = this->value = value;
	strcpy_s(this->brand, name);

	purchasing_power = perceived_value::neutral; // as is always the case when the object is created
 }


void guitar::set_perceived_value(double ratio)
{
	if (ratio <= 0.1)
		purchasing_power = perceived_value::irresistible;
	else if (ratio <= 1.5 && ratio > 0.1)
		purchasing_power = perceived_value::cheap;
	else if (ratio <= 3 && ratio > 1.5)
		purchasing_power = perceived_value::neutral;
	else if (ratio <= 5 && ratio > 3)
		purchasing_power = perceived_value::high;
	else if (ratio <= 10 && ratio > 5)
		purchasing_power = perceived_value::overpriced;
	else if (ratio > 10)
		purchasing_power = perceived_value::unattainable;
}


/*------------------------------ piano ------------------------------*/

piano::piano(piano_brands::piano_brands brand, piano_type::piano_type type, quality::quality quality)
{
	is_guitar = false;

	value = (double)(brand * type * quality);
	value /= (5*3*3); // compute ratio
	value *= 50000; // maximum piano price

	price = value;
	own_brand = brand;
	own_type = type;
	own_quality = quality;
	
	if (own_brand != 0){
		strcpy_s(this->brand, brands[own_brand - 1]);
	}

	purchasing_power = perceived_value::neutral; // as is always the case when the object is created
}


void piano::set_perceived_value(double ratio)
{
	if (ratio <= 0.1)
		purchasing_power = perceived_value::irresistible;
	else if (ratio <= 1.5 && ratio > 0.1)
		purchasing_power = perceived_value::cheap;
	else if (ratio <= 2 && ratio > 1.5)
		purchasing_power = perceived_value::neutral;
	else if (ratio <= 3 && ratio > 2)
		purchasing_power = perceived_value::high;
	else if (ratio <= 5 && ratio > 3)
		purchasing_power = perceived_value::overpriced;
	else if (ratio > 5)
		purchasing_power = perceived_value::unattainable;
}


/*------------------------------ employee ------------------------------*/


employee::employee(char * person, double value, int num) // num: 0-low; 1-neutral; 2-high
{
	strcpy_s(this->name, person);
	salary = value;
	this->skill = efficiency(num);
}


/*------------------------------ store ------------------------------*/


store::store(const store & shop)
{
	this->setting = shop.setting;
	strcpy_s(this->name, shop.name);
	reputation = shop.reputation;
	max_stock = shop.max_stock;
	value = shop.value;
	traffic = shop.traffic;
	buying_rate = shop.buying_rate;

	inventory.clear();
	staff.clear();
}

store::store(user_profile * current, char * name, int value, int num) // num: 0-poor; 1-middle; 2-rich 
{
	this->value = value;
	this->user = current;
	strcpy_s(this->name, name);

	this->setting = static_cast<area>(num);
	this->max_stock = 50 + 10 * setting; // can be altered, formula for max inventory

	inventory.clear();
	staff.clear();
}

store::~store()
{
	while (!inventory.empty())
	{
		delete (inventory.front());
		inventory.pop_front();
	}

	while (!staff.empty())
	{
		delete (staff.front());
		staff.pop_front();
	}

}

store & store::operator=(const store & shop)
{

	setting = shop.setting;
	reputation = shop.reputation;
	max_stock = shop.max_stock;
	value = shop.value;
	traffic = shop.traffic;
	buying_rate = shop.buying_rate;
	strcpy_s(this->name, shop.name);
	inventory.clear();
	staff.clear();

	return(*this);
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

void store::buy_piano(piano * piano)
{
	if (user->net_worth >= piano->value && inventory.size() < max_stock) // buying from wholesale
	{
		user->net_worth -= piano->value;
		this->inventory.push_back(piano);
		return;
	}

	// else we need to deploy a message error (warning window)
}

int store::get_max_stock()
{
	return max_stock;
}

int store::get_stock()
{
	return inventory.size();
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

void store::fire_employee(char * name)
{
	std::list<employee *>::iterator it;

	for ( it = staff.begin() ; it != staff.end(); it++)
	{

		if (std::strcmp((*it)->name, name) == 0)
		{

			delete (*it);
			*it = nullptr;
			staff.erase(it);
			return;
		}
		
	}

	// person not found
}

guitar * store::inventory_tab(int & size)
{
	size = inventory.size();
	if (size == 0) return (nullptr);

	guitar * tab = new guitar[size]; //by default, with the is_guitar we can cast it.
	std::list<instrument *>::iterator it = inventory.begin();
	guitar * ptr;

	for (int i = 0; it != inventory.end() && i < size; i++, it++)
	{
		ptr = (guitar *)*it;
		tab[i] = guitar(*ptr);
	}

	return (tab);
}

employee * store::staff_tab(int & size)
{
	size = staff.size();
	if (size == 0) return(nullptr);

	employee * tab = new employee[size];
	std::list<employee *>::iterator it = staff.begin();
	employee * ptr;

	for (int i = 0; it != staff.end() && i < size; i++, it++)
	{
		ptr = *it;
		tab[i] = employee(*ptr);

	}

	return (tab);
}

void store::fill_inventory(instrument * tab, int size)
{
	//precaution
	inventory.clear();

	for (int i = 0; i < size; i++)
	{
		if (tab[i].is_guitar)
			inventory.push_back(new guitar(tab[i].value, tab[i].brand));
		else
		{
			inventory.push_back(new piano(tab[i].get_brand(), tab[i].get_type(), tab[i].get_quality()));
		}
	}

}



void store::fill_staff(employee * tab, int size)
{
	//precaution
	staff.clear();

	for (int i = 0; i < size; i++)
	{
		staff.push_back(new employee(tab[i]));
	}
}





/*------------------------------ user_profile ------------------------------*/

user_profile::user_profile(char * name)
{
	strcpy_s(this->user, name);

	difficulty = 0;
	weekly_expenses = reputation = net_worth = 0;

}

user_profile::user_profile(const user_profile & user)
{
	strcpy_s(this->user, user.user);
	time_elapsed = user.time_elapsed;
	weekly_expenses = user.weekly_expenses;
	net_worth = user.net_worth;
	reputation = user.reputation;
	difficulty = user.difficulty;
}

user_profile::~user_profile()
{
	while (!stores.empty())
	{
		while (!stores.front()->inventory.empty())
		{
			delete stores.front()->inventory.front();
			stores.front()->inventory.pop_front();
		}

		while (!stores.front()->staff.empty())
		{
			delete stores.front()->staff.front();
			stores.front()->staff.pop_front();
		}

		delete stores.front();
		stores.pop_front();
	}
}

store * user_profile::get_active_store()
{
	return active_store;
}

void user_profile::set_active_store(store * active_store_new)
{
	active_store = active_store_new;
}

bool user_profile::set_active_store(unsigned int store_id)
{
	auto it = stores.begin();
	
	for (unsigned int c = 0; c < store_id; c++) {
		if (c >= stores.size()) return false;
		
		it = std::next(it, 1);
	}

	active_store = *it;

	return true;
}

void user_profile::buy_store(store * store)
{
	if ( this->net_worth >= store->value)
	{
		this->net_worth -= store->value;
		stores.push_back(store);
	}
}

void user_profile::save_game()
{

	if (!stores.empty())
	{
		std::list<store *>::iterator it = stores.begin();
		int	 n_stores = stores.size(), n_staff, n_inventory;

		store * store_ptr = new store[n_stores];
		store * temp = nullptr;

		guitar * inventory = nullptr;
		employee * people = nullptr;

		std::string user = user;

		for (int i = 0; i < n_stores && it != stores.end(); i++, it++)
		{
			temp = *it;

			inventory = temp->inventory_tab(n_inventory); //these now have all the inventory and staff
			people = temp->staff_tab(n_staff);

			if (inventory != nullptr)
				save_inventories(user, inventory, n_inventory, i);

			if (people != nullptr)
				save_staff(user, people, n_staff, i);

			store_ptr[i] = store(*temp); //copy does not involve lists

			if (inventory != nullptr)
			{
				delete[] inventory;
				inventory = nullptr;
			}
			if (people != nullptr)
			{
				delete[] people;
				people = nullptr;
			}
		} //store_ptr now has all stores


		if (store_ptr != nullptr) save_stores(user, store_ptr, n_stores);

		if (store_ptr != nullptr)
		{
			delete[]  store_ptr;
			store_ptr = nullptr;
		}
	} //if empty stores, it wont save any (empty stores). also, if stores file is corrupted, you'll be loaded with no store.

	std::ifstream fin("users");
	std::ofstream fout;

	if (fin.fail())
	{

		fout.open("users");

		if (fout.fail())
			error::trace_error(ErrNo::file_access);
			
	

		if (fout.is_open())
			fout << user << "\n";

		fout.close();


	}
	else //does not repeat writing the user  if already there 
	{
		std::string line, line_2 = user;
		bool is_there = false;
		while (std::getline(fin, line))
		{
			if (line_2 == line)
				is_there = true;

			if (is_there)
				break;
		}

		if (!is_there)
		{
			fout.open("users", std::ios::app);

			if (fout.fail())
				error::trace_error(ErrNo::file_access);

			fout << user << "\n";

			fout.close();
		}
	}

	fout.open(user, std::ios::binary | std::ios::trunc);

	if (fout.fail())
		error::trace_error(ErrNo::file_access);

	struct save_user save;
	save.difficulty = this->difficulty;
	save.net_worth = net_worth;
	save.reputation = reputation;
	save.weekly_expenses = weekly_expenses;
	strcpy_s(save.user, user);

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && ((&save) != nullptr))
		fout.write((char *)&save, sizeof(save_user));

	fout.close();

}

void user_profile::save_inventories(std::string  user, const  guitar * tab, int size, int store_index)
{
	std::string file_name = user;   //std structure of naming files
	file_name += ".Store_inventory";
	file_name.push_back(char(store_index));

	std::ofstream fout(file_name, std::ios::binary | std::ios::trunc); //whatever was there is redefined

	if (fout.fail())
	{
		error::trace_error(ErrNo::file_access);
	}

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && tab != nullptr)
		fout.write((char*)tab, size * sizeof(guitar));

	fout.close();
}

void user_profile::save_staff(std::string  user, const employee * tab, int size, int store_index)
{
	std::string file_name = user;   //std structure of naming files
	file_name += ".Store_staff";
	file_name.push_back(char(store_index));

	std::ofstream fout(file_name, std::ios::binary | std::ios::trunc); //whatever was there is redefined

	if (fout.fail())
	{
		error::trace_error(ErrNo::file_access);
	}

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && tab != nullptr)
		fout.write((char*)tab, size * sizeof(employee));

	fout.close();
}

void user_profile::save_stores(std::string  user, const store * tab, int size)
{
	std::string file_name = user;   //std structure of naming files
	file_name += ".Stores";

	std::ofstream fout(file_name, std::ios::binary | std::ios::trunc); //whatever was there is redefined

	if (fout.fail())
	{
		error::trace_error(ErrNo::file_access);
	}

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && tab != nullptr)
		fout.write((char*)tab, size * sizeof(store));

	fout.close();

}

void user_profile::load_game(std::string  profile_title)
{

	std::ifstream fin("users");


	if (fin.fail())
		error::trace_error(ErrNo::file_access);

	std::string line;

	while (fin.is_open())
	{
		while (std::getline(fin, line))
		{
			if (line == profile_title)
			{
				load_user(profile_title);
				fin.close();
				return;
			}
		}
	}

	error::trace_error(ErrNo::profile_not_found);

}

void user_profile::load_user(std::string & profile_title)
{

	std::ifstream fin_2(profile_title, std::ios::binary);

	if (fin_2.fail())
		error::trace_error(ErrNo::file_access);

	save_user load[1];

	if (fin_2.is_open())
	{
		fin_2.read((char *)&load, sizeof(save_user));
		fin_2.close();
	}


	strcpy_s(this->user, load[0].user);

	this->weekly_expenses = load[0].weekly_expenses;
	this->net_worth = load[0].net_worth;
	this->reputation = load[0].reputation;
	this->difficulty = load[0].difficulty;


	load_stores(this);
}

void user_profile::load_stores(user_profile * user)
{
	std::string file_name = user->user;
	file_name += ".Stores";

	std::ifstream fin(file_name, std::ios::binary);

	if (fin.fail())
		error::trace_error(ErrNo::file_access);

	int size;
	store * tab_ptr = nullptr;

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = int(fin.tellg()) / sizeof(store);
		fin.seekg(0, std::ios::beg);

		tab_ptr = (store *)malloc(size * sizeof(store));

		fin.read((char *)tab_ptr, size * sizeof(store));

		fin.close();
	}
	else
	{
		error::trace_error(ErrNo::file_access);
		return;
	}

	user->stores.clear(); //precaution

	for (int i = 0; tab_ptr != nullptr, i < size; i++)
	{


		user->stores.push_back(new store(tab_ptr[i]));

		load_store_inv(user, (*stores.back()), i);
		load_store_staff(user, (*stores.back()), i);
	}


	free(tab_ptr);
	tab_ptr = nullptr;
	
}

void user_profile::load_store_inv(const user_profile * user, store & shop, int store_index)
{
	std::string file_name = user->user;
	file_name += ".Store_inventory";
	file_name.push_back(store_index);

	std::ifstream fin(file_name, std::ios::binary);

	if (fin.fail())
		error::trace_error(ErrNo::file_access);

	int size;
	guitar * tab_ptr = nullptr;

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = int(fin.tellg()) / sizeof(guitar);
		fin.seekg(0, std::ios::beg);

		tab_ptr = (guitar *) malloc (size * sizeof(guitar));

		char  * ptr = (char *)(tab_ptr);

		fin.read(ptr, size * sizeof(guitar));

		fin.close();
	}
	else
	{
		error::trace_error(ErrNo::file_access);
		return;
	}


	shop.fill_inventory(tab_ptr, size); // this function allocates its own 

	free(tab_ptr);
	tab_ptr = nullptr;

}

void user_profile::load_store_staff(const user_profile * user, store & shop, int store_index)
{
	std::string file_name = user->user;
	file_name += ".Store_staff";
	file_name.push_back(store_index);

	std::ifstream fin(file_name, std::ios::binary);

	if (fin.fail())
		error::trace_error(ErrNo::file_access);


	int size;
	employee * tab_ptr = nullptr;

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = int(fin.tellg()) / sizeof(employee);
		fin.seekg(0, std::ios::beg);

		tab_ptr = (employee *)malloc(size * sizeof(employee));

		fin.read((char *)tab_ptr, size * sizeof(employee));

		fin.close();
	}
	else
	{
		error::trace_error(ErrNo::file_access);
		return;
	}


	shop.fill_staff(tab_ptr, size); // this function  allocates its own 


	free(tab_ptr);
	tab_ptr = nullptr;
	
}


