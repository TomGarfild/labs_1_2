#include <iostream>
using namespace std;

int gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}
struct rational
{
	int num;
	int denom;
};

bool operator==(rational a, rational b)
{
	int ga = gcd(a.num, a.denom);
	a.num /= ga; a.denom /= ga;
	int gb = gcd(b.num, b.denom);
	b.num /= gb; b.denom /= gb;
	return a.num == b.num && a.denom == b.denom;
}

void print(rational a)
{
	cout << a.num << "/" << a.denom<<" ";
}

struct elem
{
	rational data;
	elem* next;
};

rational sum_list(elem* head)
{
	rational sum = { 0, 0 };
	if (head == nullptr) return sum;
	sum.denom = 1;
	while (head->next != nullptr)
	{
		sum.num = sum.num*head->data.denom + sum.denom*head->data.num;
		sum.denom *= head->data.denom;
		head = head->next;
	}
	sum.num = sum.num * head->data.denom + sum.denom * head->data.num;
	sum.denom *= head->data.denom;
	return sum;
}

void print_list(elem* head)
{
	while (head != nullptr)
	{
		print(head->data);
		head = head->next;
	}
	cout << "\n";
}

rational get_next_rational(rational val)
{
	while (true)
	{
		if (val.num < 0)
		{
			val.num *= (-1);
			return val;
		}
		val.denom--; val.num++;
		if (val.denom == 0)
		{
			val.denom = val.num;
			val.num = 1;
		}
		if (gcd(abs(val.num), abs(val.denom)) == 1)
		{
			val.num *= (-1);
			return val;
		}
	}
}

elem* push_back(elem* head)
{
	elem* tmp = new elem;
	tmp->next = nullptr;
	if (head == nullptr)
	{
		tmp->data = { 0,1 };
		return tmp;
	}

	elem* copy_head = head;
	while (head->next != nullptr)
	{
		head = head->next;
	}
	tmp->data = get_next_rational(head->data);
	head->next = tmp;
	return copy_head;
}

int find_number(elem* head, rational val)
{
	int i = 0;
	if (head == nullptr) return -1;
	while (head->next != nullptr)
	{
		if (head->data == val) return i;
		i++;
		head = head->next;
	}
	if (head->data == val) return i;
	return -1;
}

elem* pop_first(elem* head)
{
	if (head == nullptr) return nullptr;
	elem* tmp = head;
	head = head->next;
	delete tmp;
	return head;
}

elem* find_and_delete(elem* head, rational val)
{
	if (head == nullptr) return nullptr;
	if (head->data == val) {
		cout << "Number "; print(val); cout << "was successfully deleted from the list.\n";
		return pop_first(head);
	}
	elem* head_copy = head;
	while (head->next != nullptr && !(head->next->data == val)) head = head->next;
	if (head->next != nullptr) {
		elem* to_delete = head->next;
		head->next = head->next->next;
		cout << "Number "; print(val); cout << "was successfully deleted from the list.\n";
		delete to_delete;
	}
	return head_copy;
}

int main()
{
	int n;
	cout << "Enter N: ";
	cin >> n;
	if (n < 1) return 0;
	elem* head = nullptr;
	for (int i = 0; i < n; i++)
	{
		head = push_back(head);
	}
	//Menu
	while (true)
	{
		std::cout << "Menu\n";
		std::cout << "1. Find number in the list.\n";
		std::cout << "2. Delete number in the list.\n";
		std::cout << "3. Sum of numbers in the list.\n";
		std::cout << "4. Add next number.\n";
		std::cout << "5. Print list.\n";
		int command;
		std::cin >> command;
		switch(command)
		{
		case 1:
		case 2:
			rational val;
			cout << "Enter numerator of number: ";
			cin >> val.num;
			cout << "Enter denominator of number: ";
			cin >> val.denom;
			if (val.denom < 0)
			{
				val.num *= (-1);
				val.denom *= (-1);
			}
			val.num /= gcd(abs(val.num), abs(val.denom));
			val.denom /= gcd(abs(val.num), abs(val.denom));
			if (command == 1) cout << find_number(head, val) << "\n";
			else head = find_and_delete(head, val);
			break;
		case 3:
			rational sum = sum_list(head);
			if (sum.denom == 0) cout << "Undefined\n";
			else
			{
				cout << 1.0 * sum.num / sum.denom << '\n';
			}
			break;
		case 4:
			head = push_back(head);
			break;
		case 5:
			print_list(head);
			break;
		default:
			return 0;
		}
	}
}