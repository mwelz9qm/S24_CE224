/*
Anders Ladow
Final Project class File, for "gridClass"
A* and Dijkstra's path Implementation
*/



#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm>
#include <functional> 

using namespace std;

class gridClass {
private:
    int width, height; // Grid dimensions
    vector<list<pair<int, int>>> adjList; // Adjacency list where each element is a pair (neighbor index, weight)

    //Grid will be represented by single vector
    //Say if the (x,y) coordinates are (1, 0) in a 2D array of 5x5
    //The single vector coordinate will be 5
    vector<int> grid; // Flattened grid to store node values (0 = open, 1 = obstacle)

    //Function to convert 2D grid coordinates to 1D index
    int coordToIndex(int x, int y) {
        return y * width + x;
    }

    // Function to add edges in four directions with optional weights
    void addEdges(int x, int y, int weight = 1) {
        //Changing index to single array so grid can be accessed
        int index = coordToIndex(x, y);
        //Adding left edge unless its furthest left it can be
        if (x > 0 && grid[coordToIndex(x - 1, y)] == 0) 
            adjList[index].push_back({ coordToIndex(x - 1, y), weight });
        //Adding right edge unless its furthest right it can be
        if (x < width - 1 && grid[coordToIndex(x + 1, y)] == 0) 
            adjList[index].push_back({ coordToIndex(x + 1, y), weight });
        //Adding above edge unless its highest up it can be
        if (y > 0 && grid[coordToIndex(x, y - 1)] == 0) // Up
            adjList[index].push_back({ coordToIndex(x, y - 1), weight });
        //Adding below edge unless its lowest down it can be
        if (y < height - 1 && grid[coordToIndex(x, y + 1)] == 0)
            adjList[index].push_back({ coordToIndex(x, y + 1), weight });
    }

    //Function to change single array index into 2D coordinates
    pair<int, int> indexToCoord(int index) const {
        return { index % width, index / width };
    }

    //Euclidian heuristic function, based off of "straight-line" distance between the current
    //Coordinate and the goal coordinate
    int heuristic(int index1, int index2) const {
        pair<int,int> coord1 = indexToCoord(index1); //(x,y) coordinate for current location
        pair<int,int> coord2 = indexToCoord(index2); //(x,y) coordinate for goal location
        //returning the straight line distance between the points
        return sqrt(pow((coord1.first - coord2.first),2) + pow((coord1.second - coord2.second),2));
    }

public:
    // Constructor to initialize the grid graph with dimensions and obstacles
    gridClass(int w, int h, const vector<pair<int, int>>& obstacles) : width(w), height(h), adjList(w* h), grid(w* h, 0) {
        // Initialize obstacles in the grid
        for (const auto& obs : obstacles) { //for every obstacle in the given vector
            grid[coordToIndex(obs.first, obs.second)] = 1; //change the "weight" of that index in the grid vector to 1
                                                     //as to represent an obstacle being there
        }

        // Add edges for each cell in the grid
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (grid[coordToIndex(x, y)] == 0) { // Only add edges for non-obstacle cells
                    addEdges(x, y);
                }
            }
        }
    }

    // A* search algorithm to find the shortest path from start to goal
    vector<int> aStarSearch(int startX, int startY, int goalX, int goalY) {
        int start = coordToIndex(startX, startY); //converting starting coordinates to 1D array index 
        int goal = coordToIndex(goalX, goalY); //converting goal cooordinates to 1D array index

        //Initializing the priority queue that will store what has been checked
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> path_queue;

        vector<int> costSoFar(width * height, numeric_limits<int>::max()); //Vector to store the cost as algorithm moves along the path,
                                                                     //Initialize their weights as initially very large
        vector<int> cameFrom(width * height, -1); //Vector to store the previous ideal node values, the "trail" behind the current node

        path_queue.emplace(0, start); //initialize priority_queue with the starting value
        costSoFar[start] = 0; //initializing starting cost as 0
        cameFrom[start] = start; //initializing "trail" vector

        while (!path_queue.empty()) { //while the priority queue has values 
            //current node checked is top of priority queue
            int current = path_queue.top().second; //getting index of current node
            path_queue.pop(); //remove the current value from the queue

            if (current == goal) break; //If current value is at the goal, then break loop

            for (const auto& edge : adjList[current]) { //for every edge in the adjacency list in the current node
                int next = edge.first; //getting index of next node
                int newCost = costSoFar[current] + edge.second; //Add to the cost if that location has an obstacle

                if (newCost < costSoFar[next]) { //if the new cost is less than next
                    costSoFar[next] = newCost; //change next value in cost vector to the newCost
                    int priority = newCost + heuristic(next, goal); //the new priority uses the heuristic equation defined before
                    path_queue.emplace(priority, next); //Put the new priority node into the priority_queue
                    cameFrom[next] = current; //iterating to the next value
                }
            }
        }

        // Reconstructing the resulting path
        vector<int> path; //initializing single array vector representation
        for (int at = goal; at != start; at = cameFrom[at]) {//from the goal to the start, following the "trail" vector
            if (at == -1) return {}; // No path found
            path.push_back(at); //add current location to path
        }
        path.push_back(start); //add the start value to the path
        reverse(path.begin(), path.end()); //reverse the values so they can be indexed correctly
        return path; //return the resulting path
    }


    //Function for dijkstra's algorithm to find the shortest path from start to goal, essentially the A* algorithm without the Heuristic function
    vector<int> dijkstraSearch(int startX, int startY, int goalX, int goalY) {
        int start = coordToIndex(startX, startY); //converting start coordinates to 1D array index
        int goal = coordToIndex(goalX, goalY); //converting goal coordinates to 1D array index

        //Initializing priority queue that will store the nodes that have been checked
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pathQueue;
        vector<int> costSoFar(width * height, numeric_limits<int>::max()); //vector to store cost as algorithm moves along the path
                                                                //Initialize their weights as initially very large
        vector<int> cameFrom(width * height, -1); //Vector to store the previous ideal node values, the "trail" behind the current node

        costSoFar[start] = 0; //initialize starting cost as 0
        pathQueue.emplace(0, start); //initialize priority_queue with starting location

        while (!pathQueue.empty()) { //while the priority queue has values
            int current = pathQueue.top().second; //getting index of current node
            pathQueue.pop(); //removing current node from priority_queue

            if (current == goal) break; //if current node is same as the goal, break loop

            for (const auto& edge : adjList[current]) { //for every edge in the adjacency list of current node
                int next = edge.first; //getting index of next node
                int weight = edge.second; //getting weight of next node, if its 1 then there is an obstacle
                int newCost = cameFrom[current] + weight; //add to the cost, will be nothing if there is no obstacle

                if (newCost < costSoFar[next]) { //if the new cost is less than the next
                    costSoFar[next] = newCost; //change next value in cost vector the newCost
                    cameFrom[next] = current; //iterating to the next value
                    pathQueue.emplace(costSoFar[next], next); //adding next value to the priority_queue
                }
            }
        }

        // Reconstruct path
        vector<int> path; //initializing single array vector representation
        for (int at = goal; at != -1; at = cameFrom[at]) { //from the goal to the start, following the "trail" vector
            path.push_back(at); //add current location to path
        }
        reverse(path.begin(), path.end()); //reverse path so it can be indexed correctly

        if (path.front() != start) path.clear(); // No path found
        return path; //return the resulting path
    }


    //Function to print the grid and the path from start to Goal
    void printGrid(const vector<int>& path) { //inputting the resulting path from the algorithms
        vector<char> gridDisplay(width * height, '.'); //1D array vector representation of grid, every node is a '.'
        for (int i = 0; i < grid.size(); ++i) { //for every index in the grid
            if (grid[i] == 1) { //if the value in the grid vector is 1, is an obstacle
                gridDisplay[i] = '#'; // Represent obstacle by a '#'
            }
        }
        for (int idx : path) { //for every index value in the path
            gridDisplay[idx] = '*'; // representing path by a '*'
        }
        
        //Print out the resulting 1D grid vector in 2D format
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cout << gridDisplay[coordToIndex(x, y)] << ' ';
            }
            cout << endl;
        }
    }
};

