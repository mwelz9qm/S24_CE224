/*
Anders Ladow
Main File for Final Project
A* and Dijsktra Path Grid Implementation
*/


#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <algorithm>
#include<random>
#include<set>

#include "gridClass.cpp" //importing written class file

using namespace std;

//main function
int main()
{
    int width = 50, height = 50; //initializing the width and height of the grid
    
    
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, width -1); // define the range

    //Inserting "obstacles" into the grid 
    set<pair<int, int>> obstacles; //set so values can be inserted
    while (obstacles.size() < 200) { // randomly generate given amount of obstacles
        
        
        int coord_x = distr(gen); //generating x_coordinate
        int coord_y = distr(gen); //generating y_coordinate
        
        if (coord_x == 0 && coord_y == 0) //if the coordinates are (0,0), there is no path
        {
            //so regenerate the coordinates and continue
            int coord_x = distr(gen);
            int coord_y = distr(gen);
            continue;
        }

        else if (coord_x == width-1 && coord_y == height-1) //if the coordinates are (width-1, height-1), there is no path
        {
            //so regenerate the coordinates and continue
            int coord_x = distr(gen);
            int coord_y = distr(gen);
            continue;
        }
        
        //otherwise, insert the obstacle in the grid
        obstacles.insert({ coord_x, coord_y });
        
    }

    //Changing the set of obstacles into a vector of obstacles
    vector<pair<int, int>> random_obstacles(obstacles.begin(), obstacles.end()); 

    //Creating the grid class with the specified width and height, and with the generated obstacles
    gridClass g(width, height, random_obstacles);

    int startX = 0, startY = 0, goalX = width-1, goalY = height-1; //initializing the start and end goal coordinates

    //Running dijsktra's algorithm to get the resulting path vector
    vector<int>path1 = g.dijkstraSearch(startX, startY, goalX, goalY);

    std::cout << "\nPath using Dijkstra's Search:\n";
    g.printGrid(path1); //printing the resulting grid with the path

    //Running A* algorithm, with Euclidian heuristic equation to get resulting path vector
    vector<int> path2 = g.aStarSearch(startX, startY, goalX, goalY);

    std::cout << "Path using A* Search:\n";
    g.printGrid(path2); //printing the resulting grid with the path
    
    cout << "Paths Succesfully Generated!" << endl;
    return 0;
}

