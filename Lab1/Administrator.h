#pragma once

#include "meal.h"

class Administrator
{
public:

	static unsigned int get_id();
	
	static void add_meal();

	static void delete_meal();

	static void print_meal();

	static void print_orders();
	
private:
	
	

	static void print_part(const char* message, char type, int count, meal* units);

};
