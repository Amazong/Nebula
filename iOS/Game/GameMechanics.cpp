#include "headers\GameMechanics.h"

guitar::guitar(double value, std::string name)
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
