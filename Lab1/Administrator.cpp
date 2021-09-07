#pragma once

#include "Administrator.h"
#include "order.h"
#include <iostream>

const char* path_meal = "meals.dat";
const char* path_order = "orders.dat";

unsigned int Administrator::get_id()
{
	unsigned int id = 0;
	meal unit;
	FILE* fp = fopen(path_meal, "rb");
	fseek(fp, 0, SEEK_END);
	const long pos = ftell(fp);
	if (pos > 0)
	{
		fseek(fp, pos - sizeof(meal), SEEK_SET);
		fread(&unit, sizeof(meal), 1, fp);
		id = unit.id + 1;
	}
	fclose(fp);
	return id;
}

template<class T>
T* get_data(const char* path, int& n)
{
	FILE* fp = fopen(path, "rb");
	fseek(fp, 0, SEEK_END);
	const long pos = ftell(fp);
	n = pos / sizeof(T);
	fseek(fp, 0, SEEK_SET);

	T* units = new T[n];
	fread(units, sizeof(T), n, fp);

	fclose(fp);
	return units;
}

void Administrator::add_meal()
{
	FILE* fp = fopen(path_meal, "ab");
	meal unit;
	unsigned int id = get_id();
	while (true)
	{
		unit.id = id;
		std::cout << "Enter name of the meal: ";
		std::cin >> unit.name;
		std::cout << "Enter type of the meal(h - hot, c - cold, d - dessert): ";
		std::cin >> unit.type;
		std::cout << "Enter price of the meal: ";
		std::cin >> unit.price;
		if (unit.price <= 0) break;
		fwrite(&unit, sizeof(meal), 1, fp);
		id++;
	}
	fclose(fp);
}

void Administrator::delete_meal()
{
	unsigned long delete_id;
	std::cout << "Enter id of the meal to delete: ";
	std::cin >> delete_id;
	int count;
	meal* units = get_data<meal>(path_meal, count);
	FILE* fp = fopen(path_meal, "wb");
	for (int i = 0; i < count; i++)
	{
		if (units[i].id == delete_id) continue;
		fwrite(&units[i], sizeof(meal), 1, fp);
	}
	fclose(fp);
	delete[] units;
}
void Administrator::print_meal()
{
	int count;
	meal* units = get_data<meal>(path_meal, count);
	
	print_part("Hot meals\n", 'h', count, units);

	print_part("Cold meals\n", 'c', count, units);

	print_part("Desserts\n", 'd', count, units);
	
	delete[] units;
}

void Administrator::print_part(const char* message, char type, int count, meal*units)
{
	std::cout << message;
	for (int i = 0; i < count; i++)
	{
		if (units[i].type == type)
			std::cout << "Id: " << units[i].id << " Name: " << units[i].name << " Price: " << units[i].price << '\n';
	}
	std::cout << '\n';
}

void Administrator::print_orders()
{
	int count;
	order* orders = get_data<order>(path_order, count);
	for (int i = 0; i < count; i++)
	{
		std::cout << "Id: " << orders[i].id << " Meal id: " << orders[i].meal_id << " Date: " << orders[i].order_time << '\n';
	}
	delete[] orders;
}