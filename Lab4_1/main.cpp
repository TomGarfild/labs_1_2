#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

double const e = 1e-6;

double distance(pair<int, int>& a, pair<int, int>& b)
{
	return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

vector<int> travelingSalesmanProblem(vector<vector<double>> graph, int s)
{
	int V = graph.size();
	vector<int> vertex;
	for (int i = 0; i < V; i++)
		if (i != s)
			vertex.push_back(i);

	double min_path = numeric_limits<double>::max();
	vector<int> result;
	do {
		bool ok = true;

		double current_pathweight = 0;

		int k = s;
		for (int i = 0; i < vertex.size(); i++) {
			double w = graph[k][vertex[i]];
			if (w < e) {
				ok = false;
				break;
			}
			current_pathweight += w;
			k = vertex[i];
		}
		if (graph[k][s] < e) ok = false;
		current_pathweight += graph[k][s];
		/*if (ok)
		{
			cout << current_pathweight << endl;
			cout << 1 << " ";
			for (auto x : vertex) cout << x + 1 << " ";
			cout << endl << endl;;
		}*/
		if (ok && current_pathweight < min_path)
		{
			min_path = current_pathweight;
			result = vertex;
		}

	} while (
		next_permutation(vertex.begin(), vertex.end()));

	return result;
}

const float window_size = 800;
const float move_l = 20;
int main()
{
	FILE* fp = fopen("graph1.txt", "r");
	int n, m;
	fscanf(fp, "%d", &n);
	fscanf(fp, "%d", &m);
	vector<pair<int, int>> cor(n);
	vector<vector<int>> g(n);
	vector<vector<double>> matr(n, vector<double>(n));

	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &cor[i].first);
		fscanf(fp, "%d", &cor[i].second);
	}

	for (int i = 0; i < m; i++)
	{
		int u, v;
		fscanf(fp, "%d", &u);
		fscanf(fp, "%d", &v);
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
		double d = distance(cor[u], cor[v]);
		matr[u][v] = matr[v][u] = d;
	}
	fclose(fp);
	int s = 0;
	vector<int> res = travelingSalesmanProblem(matr, s);
	res.insert(res.begin(), 0);
	for (auto x : res) cout << x+1 << " ";
	cout << endl;
	//SFML
	sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Find minimal hamilton graph");
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


	for (int i = 0; i < n; i++) {
		for (auto c : g[i]) {
			if (i == c) continue;
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(cor[i].first * 25 + move_l, window_size - cor[i].second * 25 - move_l)),
				sf::Vertex(sf::Vector2f(cor[c].first * 25 + move_l, window_size - cor[c].second * 25 - move_l))
			};

			window.draw(line, 2, sf::Lines);
		}
	}
	for (int i = 0; i < res.size(); i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(cor[res[i]].first * 25 + move_l, window_size - cor[res[i]].second * 25 - move_l)),
			sf::Vertex(sf::Vector2f(cor[res[(i+1)%n]].first * 25 + move_l, window_size - cor[res[(i+1)%n]].second * 25 - move_l))
		};
		line->color = sf::Color::Red;
		window.draw(line, 2, sf::Lines);
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