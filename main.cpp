// CE224 Final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm>
#include <functional> // for std::function
#include<random>
#include<set>

#include "gridClass.cpp"

using namespace std;

int main()
{
    int width = 25, height = 25;
    
    
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, width -1); // define the range

    set<pair<int, int>> obstacles;
    while (obstacles.size() < 150) { // Generate 5 unique obstacles
        obstacles.insert({ distr(gen), distr(gen) });
    }

    vector<pair<int, int>> random_obstacles(obstacles.begin(), obstacles.end());
    GridGraph g(width, height, random_obstacles);

    int startX = 0, startY = 0, goalX = width-1, goalY = height-1;
    vector<int> path = g.aStarSearch(startX, startY, goalX, goalY);

    std::cout << "Path using A* Search:\n";
    g.printGrid(path);

    path = g.dijkstraSearch(startX, startY, goalX, goalY);

    std::cout << "\nPath using Dijkstra's Search:\n";
    g.printGrid(path);

    return 0;
}

