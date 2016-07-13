#include "headers\GameMechanics.h"


/*------------------------------ instrument ------------------------------*/

void instrument::set_price(double price)
{
	this->price = price;
	set_perceived_value(this->price / this->value);
}

std::string instrument::print_brand_cpp()
{
	std::string b = brand;

	return b;
}

std::string instrument::print_brand_cpp_short()
{
	std::string b = brand;

	if (strcmp(brand, "Steinway & Sons") == 0) b = "S&S";

	return b;
}

std::string instrument::print_type_cpp()
{
	switch (own_type_piano) {
	case 1:
		return "digital";
	case 2:
		return "upright";
	case 3:
		return "grand";
	default:
		return "";
	}
	return std::string();
}

std::string instrument::get_value_cpp()
{
	int v = (int)(value * 100);
	std::string s = std::to_string(v/100);
	s += ".";
	
	if (v % 100 > 10) {
		s += std::to_string(v % 100);
	}
	else {
		(s += "0") += std::to_string(v % 100);
	}

	return s;
}

std::string instrument::get_price_cpp()
{
	int p = (int)(price * 100);
	std::string s = std::to_string(p / 100);
	s += ".";

	if (p % 100 > 10) {
		s += std::to_string(p % 100);
	}
	else {
		(s += "0") += std::to_string(p % 100);
	}

	return s;
}

std::string instrument::style(double d)
{
	std::string s = "";

	if (d < 0) { // negative
		s += "- ";
		s += style(-d);
		return s;
	}
	else if (d < 1000) { // display directly
		s += std::to_string((int)d).substr(0, 3);
	}
	else if (d < 1000000) { // K range
		s += std::to_string((int)d / 1000).substr(0, 3); // How many K's
		s += ".";
		s += std::to_string((int)d % 1000).substr(0, 1);
		s += "K";
	}
	else if (d < 1000000000) { // M range
		s += std::to_string((int)d / 1000000).substr(0, 3); // How many M's
		s += ".";
		s += std::to_string((int)d % 1000000).substr(0, 1);
		s += "M";
	}
	else {
		s += "1 $"; // if the user has more than 1000000000 £, we display "1 $"
					// small easter egg :)
		return s;
	}

	s += " £";

	return s;
}

std::string instrument::get_quality_cpp()
{
	switch (get_quality()) {
	case 0:
		return "N/A";
	case 1:
		return "Poor";
	case 2:
		return "Fair";
	case 3:
		return "Great";
	}
	return "";
}

std::string instrument::get_perceived_value_cpp()
{
	set_perceived_value(price / value);
	switch (get_perceived_value())
	{
	case 0:
		return "Unattainable";
	case 1:
		return "Overpriced";
	case 2:
		return "High";
	case 3:
		return "Neutral";
	case 4:
		return "Cheap";
	case 5:
		return "Irresistible";
	default:
		break;
	}
	return "";
}

/*------------------------------ guitar ------------------------------*/

guitar::guitar(double value, char * name)
{
	is_guitar = true;
	this->price = this->value = value;
	strcpy_s(this->brand, name);

	own_brand_piano = piano_brands::NA;
	own_type_piano = piano_type::NA;
	own_quality = quality::NA;

	set_perceived_value(this->price / this->value);
 }

guitar::guitar(std::string name)
{
	std::string n = name;

	bool exists = false;
	is_guitar = true;
	own_brand_piano = piano_brands::NA;
	own_type_piano = piano_type::NA;

	std::random_device rd;
	std::mt19937 random_numbers(rd());

	std::string to_check[5] = { "Larrivee", "G&L", "Martin", "PRS" ,"Maton" };

	for (int i = 0; i < 5; i++)
	{
		if (n == to_check[i])
		{
			exists = true;
			
			int low, high;

			switch (i)
			{
			case 0 :
				low = 3000;
				high = 5000;
				break;
			case 1:
				low = 800;
				high = 2500;
				break;
			case 2:
				low = 1000;
				high = 2000;
				break;
			case 3:
				low = 3000;
				high = 5000;
				break;
			case 4:
				low = 1500;
				high = 3000;
				break;
			default:
				break;
			}
			
			std::uniform_int_distribution<int> range(low, high);

			strcpy_s(this->brand, to_check[i].c_str());
			this->value = range(random_numbers);
			this->set_price(this->value);
			break;
		}
	}

	if (!exists) { // make exist
		std::uniform_int_distribution<int> name_range(0, 4);
		n = to_check[name_range(rd)];
		*this = guitar(n);
		return;
	}

	std::uniform_int_distribution<int> range(1, 3);
	
	this->own_quality = quality::quality(range(random_numbers));
}

guitar::guitar(piano_brands::piano_brands brand, piano_type::piano_type type, quality::quality quality)
{
	own_brand_piano = piano_brands::NA;
	own_type_piano = piano_type::NA;
	own_quality = quality;
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

piano::piano(double value, char * brand) {
	is_guitar = false;

	this->price = this->value = value;

	set_perceived_value(this->price/this->value);

	for (int i = 0; i < 5; i++) {
		if (strcmp(brand, piano_brands[i]) == 0) {
			own_brand_piano = static_cast<piano_brands::piano_brands>(i+1);
			strcpy_s(this->brand, brand);
			break;
		}
		own_brand_piano = piano_brands::NA;
	}

	own_quality = quality::NA;
	own_type_piano = piano_type::NA;
}

piano::piano(std::string name)
{
	piano_brands::piano_brands b;
	piano_type::piano_type t;
	quality::quality q;
	
	std::random_device rd;
	std::mt19937 random_nubers(rd());

	std::uniform_int_distribution<int> range1(1, 5);
	b = static_cast<piano_brands::piano_brands>(range1(random_nubers));
	
	std::uniform_int_distribution<int> range2(1, 3);
	t = static_cast<piano_type::piano_type>(range2(random_nubers));
	
	q = static_cast<quality::quality>(range2(random_nubers));
	
	*this = piano(b, t, q);
}

piano::piano(piano_brands::piano_brands brand, piano_type::piano_type type, quality::quality quality)
{
	is_guitar = false;

	if (brand == piano_brands::NA || type == piano_type::NA || quality == quality::NA) return;

	switch (type) { // set maximum price for type
	case 1:
		value = 1500;
		break;
	case 2:
		value = 5000;
		break;
	case 3:
		value = 50000;
		break;
	default:
		break;
	}
	
	value *= ((0.6 * brand + 0.4 * quality) / 4.2); // calculate real value

	price = value;
	own_brand_piano = brand;
	own_type_piano = type;
	own_quality = quality;
	
	if (own_brand_piano != 0){
		strcpy_s(this->brand, piano_brands[own_brand_piano - 1]);
	}

	set_perceived_value(this->price / this->value); // update purchasing power
}

void piano::set_perceived_value(double ratio)
{
	if (ratio <= 0.5)
		purchasing_power = perceived_value::irresistible;
	else if (ratio <= 1.5)
		purchasing_power = perceived_value::cheap;
	else if (ratio <= 2)
		purchasing_power = perceived_value::neutral;
	else if (ratio <= 3)
		purchasing_power = perceived_value::high;
	else if (ratio <= 5)
		purchasing_power = perceived_value::overpriced;
	else
		purchasing_power = perceived_value::unattainable;
}


/*------------------------------ employee ------------------------------*/

employee::employee(char * name)
{
	std::random_device rd;
	std::mt19937 random_numbers(rd());
	char n[51];
	strcpy_s(n, name);

	if (strcmp(name, "") == 0) {
		std::uniform_int_distribution<int> range(0, 499);

		std::ifstream names("res/text/names_500.txt");
		if (!names.is_open()) {
			error::trace_error(ErrNo::file_access);
		}
				
		int index = range(random_numbers);
		
		while (names.getline(n, 50, '\n')) {
			index--;
			if (index < 0) break;
		}
		
		n[50] = '\0'; // precaution
		
		names.close();
	}

	int e;
	double v, modulate;

	std::uniform_int_distribution<int> range1(1, 3);
	std::normal_distribution<double> range2(1, 0.2);

	e = range1(random_numbers);

	do {
		modulate = range2(random_numbers);
	} while (modulate < 0.6 || modulate > 1.4);

	v = 20000;
	v *= ((e + 1) / 3.0);
	v *= modulate;

	*this = employee(n, v, e);
}

employee::employee(char * person, double value, int eff) // eff: 1-low; 2-neutral; 3-high
{
	strcpy_s(this->name, person);
	salary = value;
	this->skill = efficiency(eff);
}

std::string employee::get_salary()
{
	int sal = (int)(salary * 100);
	std::string s = std::to_string(sal / 100);
	s += ".";

	if (sal % 100 > 10) {
		s += std::to_string(sal % 100);
	}
	else {
		(s += "0") += std::to_string(sal % 100);
	}

	return s;
}

std::string employee::get_efficiency(int num)
{
	std::string temp;

	if (num == 1)
		temp = "Low";
	else if (num == 2)
		temp = "Neutral";
	else
		temp = "High";
	return temp;
}


/*------------------------------ store ------------------------------*/


store::store(const store & shop)
{
	setting = shop.setting;
	strcpy_s(this->name, shop.name);
	reputation = shop.reputation;
	max_stock = shop.max_stock;
	value = shop.value;
	traffic = shop.traffic;
	buying_rate = shop.buying_rate;
	placement = shop.placement;

	inventory.clear();
	staff.clear();
}

store::store(user_profile * current, char * name, double value, int areacode, unsigned int max_stock, int pop) // areacode: 0-poor; 1-middle; 2-rich 
{
	this->value = value;
	this->user = current;
	this->max_stock = max_stock;
	strcpy_s(this->name, name);

	placement = static_cast<population>(pop);

	this->setting = static_cast<area>(areacode);
	this->max_stock = 50 + 10 * setting; // can be altered, formula for max inventory

	double t;
	t = 0.5 * ((3 - user->difficulty) / 3.0);
	t += 0.5 * ((placement + 1) / 3.0);
	t *= (5.0);
	
	traffic = (int)(t+0.5);

	inventory.clear();
	staff.clear();
}

store::store(user_profile * current, char * name)
{
	std::random_device rd;
	std::mt19937 random_numbers(rd());
	
	char n[51];
	strcpy_s(n, name);

	if (strcmp(name, "") == 0) {
		std::uniform_int_distribution<int> range(0, 208);

		std::ifstream names("res/text/cities_209.txt");
		if (!names.is_open()) {
			error::trace_error(ErrNo::file_access);
		}

		int index = range(random_numbers);

		while (names.getline(n, 50, '\n')) {
			index--;
			if (index < 0) break;
		}

		n[50] = '\0'; // precaution

		names.close();
	}

	int a, p;
	unsigned int s;
	double v, modulate;	

	std::uniform_int_distribution<int> range1(1, 3);
	std::normal_distribution<double> range2(1, 0.2);
	
	a = static_cast<area>(range1(random_numbers));
	p = static_cast<population>(range1(random_numbers));

	do {
		modulate = range2(random_numbers);
	} while (modulate < 0.6 || modulate > 1.4);
	
	v = 300000;
	v *= ((a+1) / 3.0) * ((p+1) / 3.0);
	v *= modulate;

	std::uniform_int_distribution<int> stock_range(50, 100);

	s = stock_range(random_numbers);
	s = (int)((double)s * (((a + 1) / 3.0) * (current->difficulty + 1) / 3.0));
	s = (int)((double)s * modulate);

	*this = store(current, n, v, a, s, p);
	this->user = current;
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
	placement = shop.placement;

	return(*this);
}

std::string store::get_area(int num)
{
	std::string temp = "";

	if (num == 0)
		temp = "Poor";
	else if (num == 1)
		temp = "Middle";
	else
		temp = "Rich";

	return temp;
}

std::string store::get_population(int num)
{
	std::string temp = "";

	if (num == 0)
		temp = "Rural";
	else if (num == 1)
		temp = "Town";
	else
		temp = "City";

	return temp;
}

std::string store::get_buying_rate()
{
	if (buying_rate < 0) return "0";
	return std::to_string(buying_rate);
}

// averages
void store::update_average_purchasing_power() {
	double purchasing_power = 0;
	int n = 0;

	for (auto it = inventory.begin(); it != inventory.end(); it++) {
		purchasing_power += (*it)->get_perceived_value();
		n++;
	}
	
	if (n == 0) {
		average_purchasing_power = -1;
		return;
	}

	average_purchasing_power = purchasing_power / n;
}

void store::update_average_efficiency() {
	double tot_efficiency = 0;
	int n = 0;

	for (auto it = staff.begin(); it != staff.end(); it++) {
		tot_efficiency += (*it)->skill;
		n++;
	}

	if (n == 0) {
		average_efficiency = -1;
		return;
	}

	average_efficiency = tot_efficiency / n;
}

bool store::buy_guitar(guitar * guitar)
{
	if (user->net_worth >= guitar->value && inventory.size() < max_stock) // buying from wholesale
	{
		user->net_worth -= guitar->value;
		this->inventory.push_back(guitar);
		
		LOGGER::log("Bought " + guitar->print_brand_cpp() + " guitar for " + guitar->get_value_cpp() + "£ (peanuts)");
		return true;
	}

	return false;
}

bool store::buy_piano(piano * piano)
{
	if (user->net_worth >= piano->value && inventory.size() < max_stock) // buying from wholesale
	{
		user->net_worth -= piano->value;
		this->inventory.push_back(piano);

		LOGGER::log("Bought " + piano->print_type_cpp() + " " + piano->print_brand_cpp() + " piano for " + piano->get_value_cpp() + "£ (peanuts)");
		return true;
	}

	return false;
}

bool store::run_probability(double prob)
{
	std::mt19937 random_numbers(rd());

	std::uniform_real_distribution<double> range(0.0, 1.0);

	double number = range(random_numbers);

	if (number > prob) return false;
	return true;
}

void store::update_traffic()
{
	double weeks_in_year = user->time_elapsed.asSeconds();
	weeks_in_year = ((int)weeks_in_year % 520) / 10;

	double traffic_double = 2.5 * (1 + reputation) + 2 * cos((weeks_in_year / 26) * (atan(1) * 4));
	
	switch (user->difficulty)
	{
	case 0:
		traffic_double += 1;
		break;
	case 1:
		break;
	case 2:
		traffic_double -= 1;
		break;
	default:
		break;
	}

	if (traffic_double < 0) {
		traffic = 0;
		return;
	}

	traffic_double += 0.5;
	traffic = (int)traffic_double;
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
		int product_pos;
		int employee_pos;

		double desirability, service;

		update_traffic();
		
		for (int i = traffic; i > 0; i--)
		{
			std::uniform_int_distribution<int> inventory_range(0, (inventory.size() - 1)); // random position to evaluate
			std::uniform_int_distribution<int> staff_range(0, (staff.size() - 1)); // random staff member to help

			product_pos = inventory_range(random_numbers); // position 0 -> size-1
			employee_pos = staff_range(random_numbers); // position 0 -> size-1

			// do the math
			update_averages();

			std::list<instrument *>::iterator instrument_it = inventory.begin();
			for (int j = product_pos; j > 0; j--) {
				instrument_it++;
			}
			desirability = ((setting + 1)/3.0) * (0.3 * (get_average_purchasing_power() / 5.0) + 0.7 * ((*instrument_it)->get_perceived_value()) / 5.0);
			desirability *= 0.6;

			std::list<employee *>::iterator employee_it = staff.begin();
			for (int j = employee_pos; j > 0; j--) {
				employee_it++;
			}
			service = 0.3 * (get_average_efficiency() / 3.0) + 0.7 * ((*employee_it)->skill / 3.0);
			service *= 0.4;

			double probability = desirability + service;

			if (run_probability(probability)) {
				if ((*instrument_it)->is_guitar) {
					LOGGER::log("Sold " + (*instrument_it)->print_brand_cpp() + " guitar for " + (*instrument_it)->get_price_cpp() + "£ (peanuts)");
				}
				else {
					LOGGER::log("Sold " + (*instrument_it)->print_type_cpp() + " " + (*instrument_it)->print_brand_cpp() + " piano for " + (*instrument_it)->get_price_cpp() + "£ (peanuts)");
				}

				sell_instrument(product_pos); // to be redefined upon piano class deployment	
				
				if (inventory.empty() || staff.empty()) {
					for (int j = i; j > 0; j--) {
						reputation += 1;
						reputation *= 0.99;
						reputation -= 1;
					}
					LOGGER::log("Inventory emptied"); 
					LOGGER::log("Your inventory is empty. Your reputation has decreased");
					break;
				}
				continue;
			}
			reputation += 1;
			reputation *= 0.99;
			reputation -= 1;
			LOGGER::log("The client didn't buy anything. Your reputation has decreased");
		}
	}

	else {
		reputation += 1;
		reputation *= 0.99;
		reputation -= 1;
		LOGGER::log("Your inventory is empty. Reputation has decreased");
		// deploy message your inventory is empty. (else)
	}
}

void store::sell_instrument(int position_offset)
{
	std::list<instrument *>::iterator it = inventory.begin();

	for ( ; position_offset != 0; position_offset--) // proceding to position offset
	{
		it++;
	}

	// *it -> pointer to guitar
	user->net_worth += (*it)->price;
	reputation += 1;
	reputation *= 1.01;
	reputation -= 1;
	LOGGER::log("Sold an item! Your reputation has increased");
	if (reputation > 1) reputation = 1;

	delete (*it); // memory management

	inventory.erase(it);
}

bool store::hire_employee(employee * employee)
{
	if (user->get_balance() < employee->salary) {
		return false;
	}
	
	staff.push_back(employee); 
	LOGGER::log("Hired " + employee->get_name() + " for " + employee->get_salary() + "£ (peanuts)");
	return(true);
}

void store::fire_employee(char * name)
{
	std::list<employee *>::iterator it;

	for ( it = staff.begin() ; it != staff.end(); it++)
	{
		if (std::strcmp((*it)->name, name) == 0)
		{
			LOGGER::log("Fired " + (*it)->get_name() + ". Will not be missed");
			
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
			inventory.push_back(new piano(tab[i].get_brand_piano(), tab[i].get_type_piano(), tab[i].get_quality()));
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

std::string store::get_value_cpp()
{
	int v = (int)(value * 100);
	std::string s = std::to_string(v / 100);
	s += ".";

	if (v % 100 > 10) {
		s += std::to_string(v % 100);
	}
	else {
		(s += "0") += std::to_string(v % 100);
	}

	return s;
}


/*------------------------------ user_profile ------------------------------*/

user_profile::user_profile(char * name)
{
	strcpy_s(this->user, name);

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
	time_elapsed = user.time_elapsed;
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
	if (active_store == nullptr)
	{
		active_store = stores.front();
	}
	return active_store;
}

bool user_profile::set_active_store(store * active_store_new)
{
	if (!stores.empty()){
		active_store = active_store_new;
		return true;
	}
	return false;
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
	if (this->net_worth >= store->value)
	{
		LOGGER::log("Bought " + store->get_name_cpp() + " for " + store->get_value_cpp() + "£ (peanuts)");
		this->net_worth -= store->value;
		stores.push_back(store);
	}
}

std::string user_profile::get_time_str() const
{
	std::string t;

	int years = ((int)time_elapsed.asSeconds()) / (int)YEAR_TIME_SECS;

	t += std::to_string(years);
	t += "Y  ";

	int weeks = (((int)time_elapsed.asSeconds()) % (int)YEAR_TIME_SECS) / (int)WEEK_TIME_SECS;

	t += std::to_string(weeks);
	t += "W";

	return t;
}

void user_profile::save_game()
{
	std::string user = "saves/";
	if (!stores.empty())
	{
		std::list<store *>::iterator it = stores.begin();
		int	n_stores = stores.size(), n_staff, n_inventory;

		store * store_ptr = new store[n_stores];
		store * temp = nullptr;

		guitar * inventory = nullptr;
		employee * people = nullptr;

		user += this->user;

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
			delete[] store_ptr;
			store_ptr = nullptr;
		}
	} //if empty stores, it wont save any (empty stores). also, if stores file is corrupted, you'll be loaded with no store.

	std::ifstream fin("saves/users");
	std::ofstream fout;

	if (fin.fail())
	{
		fout.open("saves/users");

		if (fout.fail())
			error::trace_error(ErrNo::file_access);
			
		if (fout.is_open())
			fout << this->user << "\n";

		fout.close();
	}
	else //does not repeat writing the user if already there 
	{
		std::string line, line_2 = this->user;
		bool is_there = false;
		while (std::getline(fin, line))
		{
			if (line_2 == line) {
				is_there = true;
				break;
			}
		}

		if (!is_there)
		{
			fout.open("saves/users", std::ios::app);

			if (fout.fail())
				error::trace_error(ErrNo::file_access);

			fout << this->user << "\n";

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
	save.time_elapsed = time_elapsed.asSeconds();
	strcpy_s(save.user, this->user);

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && ((&save) != nullptr))
		fout.write((char *)&save, sizeof(save_user));

	LOGGER::log("Game " + get_name_cpp() + " saved");

	fout.close();
}

void user_profile::save_inventories(std::string user, const guitar * tab, int size, int store_index)
{
	std::string file_name = user; //std structure of naming files
	file_name += ".Store_inventory";
	file_name.push_back(char(store_index + 65));

	std::ofstream fout(file_name, std::ios::binary | std::ios::trunc); //whatever was there is redefined

	if (fout.fail())
	{
		error::trace_error(ErrNo::file_access);
	}

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && tab != nullptr)
		fout.write((char*)tab, size * sizeof(guitar));

	fout.close();

	CryptoKey k(user);
	CryptoFile f(file_name, statuses::plaintext);

	f.encrypt(file_name + "_encrypted", k);
}

void user_profile::save_staff(std::string user, const employee * tab, int size, int store_index)
{
	std::string file_name = user; //std structure of naming files
	file_name += ".Store_staff";
	file_name.push_back(char(store_index + 65));

	std::ofstream fout(file_name, std::ios::binary | std::ios::trunc); //whatever was there is redefined

	if (fout.fail())
	{
		error::trace_error(ErrNo::file_access);
	}

	fout.seekp(0, std::ios::beg);

	if (fout.is_open() && tab != nullptr)
		fout.write((char*)tab, size * sizeof(employee));

	fout.close();

	CryptoKey k(user);
	CryptoFile f(file_name, statuses::plaintext);

	f.encrypt(file_name + "_encrypted", k);
}

void user_profile::save_stores(std::string user, const store * tab, int size)
{
	std::string file_name = user; //std structure of naming files
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

	CryptoKey k(user);
	CryptoFile f(file_name, statuses::plaintext);

	f.encrypt(file_name + "_encrypted", k);
}

void user_profile::load_game(std::string profile_title)
{
	std::ifstream fin("saves/users");

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

				LOGGER::log("Game " + get_name_cpp() + " loaded");

				fin.close();
				return;
			}
		}
	}

	error::trace_error(ErrNo::profile_not_found);
}

void user_profile::load_user(std::string & profile_title)
{	
	std::ifstream fin_2("saves/" + profile_title, std::ios::binary);

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
	this->time_elapsed = sf::Time(sf::seconds(load[0].time_elapsed));		
	
	load_stores(this);
}

void user_profile::load_stores(user_profile * user)
{
	std::string file_name = "saves/";
	file_name += user->user;
	file_name += ".Stores";

	CryptoKey k(user->get_name_cpp());
	CryptoFile f(file_name + "_encrypted", statuses::encrypted);

	f.decrypt(file_name, k);

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

		user->stores.back()->user = user;

		load_store_inv(user, (*stores.back()), i);
		load_store_staff(user, (*stores.back()), i);
	}

	free(tab_ptr);
	tab_ptr = nullptr;
}

void user_profile::load_store_inv(const user_profile * user, store & shop, int store_index)
{
	std::string file_name = "saves/";
	file_name += user->user;
	file_name += ".Store_inventory";
	file_name.push_back(store_index + 65);

	CryptoKey k(user->get_name_cpp());
	CryptoFile f(file_name + "_encrypted", statuses::encrypted);

	f.decrypt(file_name, k);

	std::ifstream fin(file_name, std::ios::binary);

	if (fin.fail())
	{
		fin.close();
		return;//error::trace_error(ErrNo::file_access);
	}
	int size;
	guitar * tab_ptr = nullptr;

	if (fin.is_open())
	{
		fin.seekg(0, std::ios::end);
		size = int(fin.tellg()) / sizeof(guitar);
		fin.seekg(0, std::ios::beg);

		tab_ptr = (guitar *) malloc (size * sizeof(guitar));

		char * ptr = (char *)(tab_ptr);

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
	std::string file_name = "saves/";
	file_name += user->user;
	file_name += ".Store_staff";
	file_name.push_back(store_index + 65);

	CryptoKey k(user->get_name_cpp());
	CryptoFile f(file_name + "_encrypted", statuses::encrypted);

	f.decrypt(file_name, k);

	std::ifstream fin(file_name, std::ios::binary);

	if (fin.fail())
	{
		fin.close();
		return;//error::trace_error(ErrNo::file_access);
	}
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
	
	shop.fill_staff(tab_ptr, size); // this function allocates its own 
	
	free(tab_ptr);
	tab_ptr = nullptr;	
}

std::string user_profile::get_balance_styled(int arg) const
{
	// Displays balance, according to predetermined style:
	// For example, if (net_worth == 11234), will return 11.2K £

	if (arg == 0) {
		arg = (int)net_worth;
	}

	std::string s = "";

	if (arg < 0) { // negative
		s += "- ";
		s += get_balance_styled(-arg);
		return s;
	}
	else if (arg < 1000) { // display directly
		s += std::to_string(arg).substr(0, 3);
	}
	else if (arg < 1000000) { // K range
		s += std::to_string(arg / 1000).substr(0, 3); // How many K's
		s += ".";
		s += std::to_string(arg % 1000).substr(0, 1);
		s += "K";
	}
	else if (arg < 1000000000) { // M range
		s += std::to_string(arg / 1000000).substr(0, 3); // How many M's
		s += ".";
		s += std::to_string(arg % 1000000).substr(0, 1);
		s += "M";
	}
	else {
		s += "1 $"; // if the user has more than 1000000000 £, we display "1 $"
					// small easter egg :)
		return s;
	}

	s += " £";

	return s;
}
