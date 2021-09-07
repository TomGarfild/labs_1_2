#include "meal.h"
#include "order.h"
#include <iostream>
#include <ctime>

const bool admin = false;
const char* path_meal = "meals.dat";
const char* path_order = "orders.dat";

unsigned int get_id(const char* path);

template<class T>
T* get_data(const char* path, int& n);

void add_meal();

void delete_meal();

void print_meal();

void print_part(const char* message, char type, int count, meal* units);

void print_orders();

double make_order(FILE* fp, meal* units, int n, unsigned int id);

int main()
{
	
	if (admin)
	{
		while (true)
		{
			std::cout << "Menu\n";
			std::cout << "1. Add meal\n";
			std::cout << "2. Delete meal\n";
			std::cout << "3. Print orders\n";
			int command;
			std::cin >> command;
			if (command == 1)
			{
				add_meal();
			}
			else if (command == 2)
			{
				delete_meal();
			}
			else if (command == 3)
			{
				print_orders();
			}
			else break;
		}
	}
	else
	{
		int n;
		meal* units = get_data<meal>("meals.dat", n);
		FILE* fp = fopen("orders.dat", "ab");
		double sum = 0;
		FILE* f = fopen(path_order, "rb");
		fseek(f, 0, SEEK_END);
		const long pos = ftell(f);
		fclose(f);
		unsigned int id = 0;
		if (pos > 0) id = pos / sizeof(order);
		while (true)
		{
			std::cout << "Menu\n";
			std::cout << "1. Make order\n";
			std::cout << "2. Print meals\n";
			int command;
			std::cin >> command;
			if (command == 1)
			{
				sum += make_order(fp, units, n, id);
				id++;
			}
			else if (command == 2)
			{
				print_meal();
			}
			else break;
		}
		if (sum > 0)
		{
			std::cout << "Your order will be cost: " << sum << '\n';
		}
		fclose(fp);
	}
	return 0;
}

unsigned int get_id(const char* path)
{
	unsigned int id = 0;
	meal unit;
	FILE* fp = fopen(path, "rb");
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

void add_meal()
{
	FILE* fp = fopen(path_meal, "ab");
	meal unit;
	unsigned int id = get_id(path_meal);
	unit.id = id;
	std::cout << "Enter name of the meal: ";
	std::cin >> unit.name;
	std::cout << "Enter type of the meal(h - hot, c - cold, d - dessert): ";
	std::cin >> unit.type;
	std::cout << "Enter price of the meal: ";
	std::cin >> unit.price;
	if (unit.price <= 0) return;
	std::cout << "Meal was successfully added with id: " << unit.id << '\n';
	fwrite(&unit, sizeof(meal), 1, fp);
	fclose(fp);
}

void delete_meal()
{
	unsigned long delete_id;
	std::cout << "Enter id of the meal to delete: ";
	std::cin >> delete_id;

	std::cout << "Are you sure about deleting meal with id: " << delete_id << ". Enter 1 if yes.\n";
	int ans;
	std::cin >> ans;
	if (ans != 1) return;
	
	int count;
	meal* units = get_data<meal>(path_meal, count);
	FILE* fp = fopen(path_meal, "wb");
	bool has = false;
	for (int i = 0; i < count; i++)
	{
		if (units[i].id == delete_id) {
			has = true;
			continue;
		}
		fwrite(&units[i], sizeof(meal), 1, fp);
	}
	if (has) std::cout << "Meal with id: " << delete_id << " was successfully deleted.\n";
	else std::cout << "Meal with id: " << delete_id << " doesn't exist.\n";
	fclose(fp);
	delete[] units;
}

void print_meal()
{
	int count;
	meal* units = get_data<meal>(path_meal, count);

	if (count == 0)
	{
		std::cout << "Sorry, but many is empty.\n";
	}

	print_part("Hot meals\n", 'h', count, units);

	print_part("Cold meals\n", 'c', count, units);

	print_part("Desserts\n", 'd', count, units);

	delete[] units;
}

void print_part(const char* message, char type, int count, meal* units)
{
	std::cout << message;
	for (int i = 0; i < count; i++)
	{
		if (units[i].type == type)
			std::cout << "Id: " << units[i].id << " Name: " << units[i].name << " Price: " << units[i].price << '\n';
	}
	std::cout << '\n';
}

void print_orders()
{
	int count;
	order* orders = get_data<order>(path_order, count);
	if (count == 0) std::cout << "Sorry, but you don't have any orders.\n";
	for (int i = 0; i < count; i++)
	{
		std::cout << "Id: " << orders[i].id << " Meal id: " << orders[i].meal_id << " Date: " << orders[i].order_time << '\n';
	}
	delete[] orders;
}

double make_order(FILE* fp, meal* units, int n, unsigned int id)
{
	std::cout << "Enter id of the meal that you want to order: ";
	unsigned int meal_id;
	std::cin >> meal_id;
	for (int i = 0; i < n; i++)
	{
		if (units[i].id == meal_id)
		{
			std::cout << "Enter how many portions of the meal do you want to order: ";
			unsigned int count;
			std::cin >> count;
			std::cout << "Your order will be cost: " << count * units[i].price << '\n';
			
			order ord{ id, meal_id, count };
			std::time_t now = std::time(nullptr);
			std::tm* ptm = std::localtime(&now);
			std::strftime(ord.order_time, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
			fwrite(&ord, sizeof(ord), 1, fp);
			return count*units[i].price;
		}
	}
	std::cout << "Sorry, but meal with such id doesn't exist.\n";
	return 0;
}