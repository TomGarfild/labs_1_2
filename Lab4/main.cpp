#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

struct triangle
{
	int x, y, z;
	double square;
};

vector<triangle> ans;
const int depth = 3;

double calc_square(double a, double b, double c)
{
	double p = (a + b + c) / 2;
	return sqrt(p * (p - a) * (p - b) * (p - c));
}

double distance(pair<int, int>& a, pair<int,int>& b)
{
	return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

bool operator==(triangle& a, triangle& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator<(triangle& a, triangle& b)
{
	return a.square < b.square;
}

void dfs(int v, int curr, vector<int> list, vector<vector<int>>& g, vector<char>& used, vector<pair<int, int>>& cor)
{
	for (auto x : g[v])
	{
		if (curr == depth && x == list[0])
		{
			triangle t;
			t.x = list[0]; t.y = list[1]; t.z = list[2];
			if (t.x > t.y) swap(t.x, t.y);
			if (t.y > t.z) swap(t.y, t.z);
			if (t.x > t.y) swap(t.x, t.y);
			double a = distance(cor[list[0]], cor[list[1]]);
			double b = distance(cor[list[1]], cor[list[2]]);
			double c = distance(cor[list[0]], cor[list[2]]);
			t.square = calc_square(a, b, c);
			bool ok = true;
			for (auto a : ans)
			{
				if (a == t) { ok = false; break; }
			}
			if (ok) ans.push_back(t);
			return;
		}
		if (used[x] || curr == depth) continue;
		used[x] = true;
		list.push_back(x);
		dfs(x, curr + 1, list, g, used, cor);
		list.pop_back();
		used[x] = false;
	}
}

void bubble_sort(vector<triangle>& a)
{
	int n = a.size();
	for (int i = 0; i < n - 1; i++)
		for (int j = 0; j < n - i - 1; j++)
			if (a[j + 1] < a[j])
				swap(a[j], a[j + 1]);
}

void insert_sort(vector<triangle>& a)
{
	int n = a.size();
	for (int i = 1; i < n; i++)
	{
		triangle t = a[i];
		int j = i - 1;

		while (j >= 0 && t < a[j])
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = t;
	}
}

const float window_size = 800;
const float move_l = 20;
int main()
{
	FILE* fp = fopen("graph2.txt", "r");
	int n, m;
	fscanf(fp, "%d", &n);
	fscanf(fp, "%d", &m);
	vector<pair<int, int>> cor(n + 1);
	vector<vector<int>> g(n+1);
	vector<char> used(n+1);

	for (int i = 1; i <= n; i++)
	{
		fscanf(fp, "%d", &cor[i].first);
		fscanf(fp, "%d", &cor[i].second);
	}
	
	for (int i = 0; i < m; i++)
	{
		int u, v;
		fscanf(fp, "%d", &u);
		fscanf(fp, "%d", &v);
		g[u].push_back(v);
		g[v].push_back(u);
	}
	fclose(fp);
	for (int i = 1; i <= n; i++)
	{
		used[i] = true;
		dfs(i, 1, {i}, g, used, cor);
		used[i] = false;
	}

	for (auto a : ans)
	{
		cout << a.x << " " << a.y << " " << a.z << " " << a.square << endl;
	}
	cout << endl;

	
	bubble_sort(ans);
	//insert_sort(ans);
	
	for (auto a : ans)
	{
		cout << a.x << " " << a.y << " " << a.z << endl;
	}

	//SFML
	sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Find cycles and sort");
	/*sf::RectangleShape vline(sf::Vector2f(window_size, 5.f));
	vline.setOrigin(sf::Vector2f(0.f,2.5f));
	vline.setPosition(sf::Vector2f(move_l, 0.f));
	vline.rotate(90.f);
	window.draw(vline);

	sf::RectangleShape hline(sf::Vector2f(window_size, 5.f));
	hline.setOrigin(sf::Vector2f(0.f, 2.5f));
	hline.setPosition(sf::Vector2f(0.f, window_size- move_l));
	window.draw(hline);

	for (float i = 40; i < window_size; i += 20) {
		sf::RectangleShape line(sf::Vector2f(15.f, 2.f));
		line.setOrigin(sf::Vector2f(0.f, 7.5f));
		line.setPosition(sf::Vector2f(12.5f, i+7.5));
		window.draw(line);

		line.rotate(90.f);
		line.setPosition(sf::Vector2f(i-7.5, window_size - 27.5f));
		window.draw(line);
	}*/

	
	for (int i = 1; i <= n; i++) {
		for (auto c : g[i]) {
			if (i == c) continue;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(cor[i].first*25+move_l, window_size-cor[i].second*25-move_l)),
				sf::Vertex(sf::Vector2f(cor[c].first * 25 + move_l, window_size - cor[c].second * 25 - move_l))
			};

			window.draw(line, 2, sf::Lines);
		}
	}
	for (auto res : ans)
	{
		sf::Vertex line[2] =
		{
			sf::Vertex(sf::Vector2f(cor[res.x].first * 25 + move_l, window_size - cor[res.x].second * 25 - move_l)),
			sf::Vertex(sf::Vector2f(cor[res.y].first * 25 + move_l, window_size - cor[res.y].second * 25 - move_l))
		};
		line->color = sf::Color::Red;
		window.draw(line, 2, sf::Lines);
		
		sf::Vertex line1[2] =
		{
			sf::Vertex(sf::Vector2f(cor[res.x].first * 25 + move_l, window_size - cor[res.x].second * 25 - move_l)),
			sf::Vertex(sf::Vector2f(cor[res.z].first * 25 + move_l, window_size - cor[res.z].second * 25 - move_l))
		};
		line1->color = sf::Color::Red;
		window.draw(line1, 2, sf::Lines);
		
		sf::Vertex line2[2] =
		{
			sf::Vertex(sf::Vector2f(cor[res.z].first * 25 + move_l, window_size - cor[res.z].second * 25 - move_l)),
			sf::Vertex(sf::Vector2f(cor[res.y].first * 25 + move_l, window_size - cor[res.y].second * 25 - move_l))
		};
		line2->color = sf::Color::Red;
		window.draw(line2, 2, sf::Lines);
	}
	for (auto c : cor)
	{
		sf::CircleShape shape(10.f);
		shape.setFillColor(sf::Color::Green);
		shape.setOrigin(sf::Vector2f(10.f, 10.f));
		shape.setPosition(sf::Vector2f(c.first * 25 + move_l, window_size - c.second * 25 - move_l));
		window.draw(shape);
	}
	window.display();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


	}
	return 0;
}