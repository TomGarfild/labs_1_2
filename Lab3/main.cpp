#include <iostream>
using namespace std;

struct et
{
	string value;
	et* left, * right;
};

et* newNode(const string& v)
{
	et* temp = new et;
	temp->left = temp->right = nullptr;
	temp->value = v;
	return temp;
}

int get_priority(const string& s)
{
	int level = 0;
	int p = 3;
	int len = s.length();
	for (int i = 0; i < len; i++)
	{
		if (s[i] == '(')
		{
			level++;
		}
		else if (level == 0 && (s[i] == '+' || s[i] == '-'))
		{
			return 0;
		}
		else if (level == 0 && s[i] == '*')
		{
			p = 1;
		}
		else if (level == 0 && i <= len - 3 && (s.substr(i, 3) == "sin" || s.substr(i, 3) == "cos"))
		{
			p = min(p, 2);
		}
		else if (s[i] == ')')
		{
			level--;
		}
	}
	return p;
}

et* build_tree(string& s)
{

	int p = get_priority(s);
	int len = s.length();
	int level = 0;
	
	
	for (int i = 0; i < len; i++)
	{
		if (s[i] == '(')
		{
			level++;
		}
		else if (level == 0 && ((p == 0 && (s[i] == '+' || s[i] == '-')) || (p == 1 && s[i] == '*')))
		{
			string v = string(1, s[i]);
			et* root = newNode(v);
			string ltmp = s.substr(0, i);
			root->left = build_tree(ltmp);
			string rtmp = s.substr(i + 1, len - i - 1);
			root->right = build_tree(rtmp);
			return root;
		}
		else if (level == 0 && p == 2 && i <= len - 3 && (s.substr(i, 3) == "sin" || s.substr(i, 3) == "cos"))
		{
			string v =  s.substr(i, 3);
			et* root = newNode(v);
			string ltmp = s.substr(4, len-5);
			root->left = build_tree(ltmp);
			return root;
		}
		else if (s[i] == ')')
		{
			level--;
		}
		else if (p == 3)
		{
			if (s[0] == '(')
			{
				string tmp = s.substr(1, len - 2);
				return build_tree(tmp);
			}
			return newNode(s);
		}
		
	}
	
	return nullptr;
}

et* diff(et* t, string v)
{
	if (t->right == nullptr && t->left == nullptr)
	{
		if (t->value == v)
		{
			return newNode("1");
		}
		else
		{
			return newNode("0");
		}
		
	}
	else if (t-> value == "+" || t-> value == "-")
	{
		et* tcopy = newNode(t->value);
		tcopy->left = diff(t->left, v);
		tcopy->right = diff(t->right, v);
		return tcopy;
	}
	else if (t-> value == "*")
	{
		et* tcopy = newNode("+");
		et* dl = diff(t->left, v);
		et* dr = diff(t->right, v);
		et* tl = newNode("*");
		tl->left = dl;
		tl->right = t->right;
		et* tr = newNode("*");
		tr->left = dr;
		tr->right = t->left;
		tcopy->left = tl;
		tcopy->right = tr;
		return tcopy;
	}
	else if (t->value == "sin")
	{
		et* tcopy = newNode("*");
		tcopy->right = newNode("cos");
		tcopy->right->left = t->left;
		tcopy->left = diff(t->left, v);
		return tcopy;
	}
	else if (t-> value == "cos")
	{
		et* tcopy = newNode("-");
		tcopy->right = newNode("*");
		tcopy->right->right = newNode("sin");
		et* tl = t->left;
		tcopy->right->right->left = tl;
		tcopy->right->left = diff(t->left, v);
		tcopy->left = newNode("0");
		return tcopy;
	}
}

string inorder(et* t)
{
	if (t != nullptr)
	{
		if (t->right == nullptr && t->left == nullptr) return t->value;

		if (t->right == nullptr)
		{
			return t->value + "(" + inorder(t->left) + ")";
		}

		if (t->value == "*")
		{
			if (t->left->value == "0" || t->right->value == "0") return "0";
			if (t->left->value == "1") return inorder(t->right);
			if (t->right->value == "1") return inorder(t->left);
		}
		string ans = inorder(t->left) + t->value + inorder(t->right);
		if (ans[0] == '(' && ans.back() == ')')
		{
			return ans;
		}
		return "(" + ans + ")";
	}
}

int main() {
	string expression;
	cout << "Enter expression: ";
	cin >> expression;
	string var;
	cout << "Enter variable name by wich you want to differentiate your expression: ";
	cin >> var;

	/*
	*Rules
	* Diff(C) = 0
	* Diff(V) = 0
	* Diff(var) = 1
	* Diff(x+y) = Diff(x) + Diff(y)
	* Diff(x-y) = Diff(x) - Diff(y)
	* Diff(x*y) = Diff(x)*y + x*Diff(y)
	* Diff(sin(expr)) = cos(expr)*Diff(expr)
	* Diff(cos(expr)) = -sin(expr)*Diff(expr)
	*/

	//TODO: Build a binary expression tree representing the expression.
	//TODO: Differentiate the expression tree with respect to the variable.
	//TODO: Simplify the expression tree representing the derivative.
	//TODO: Print the expression corresponding to the simplified tree.

	et* tree = build_tree(expression);
	et* dtree = diff(tree, var);
	cout << inorder(dtree);
	return 0;
}