#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm>
#include <functional> // for std::function

using namespace std;

class GridGraph {
private:
    int width, height;     // Grid dimensions
    vector<list<pair<int, int>>> adjList; // Adjacency list where each element is a pair (neighbor index, weight)
    vector<int> grid; // Flattened grid to store node values (0 = open, 1 = obstacle)

    // Converts 2D grid coordinates to 1D index
    int coordToIndex(int x, int y) {
        return y * width + x;
    }

    // Adds edges in four directions with optional weights
    void addEdges(int x, int y, int weight = 1) {
        int index = coordToIndex(x, y);
        if (x > 0 && grid[coordToIndex(x - 1, y)] == 0) // Left
            adjList[index].push_back({ coordToIndex(x - 1, y), weight });
        if (x < width - 1 && grid[coordToIndex(x + 1, y)] == 0) // Right
            adjList[index].push_back({ coordToIndex(x + 1, y), weight });
        if (y > 0 && grid[coordToIndex(x, y - 1)] == 0) // Up
            adjList[index].push_back({ coordToIndex(x, y - 1), weight });
        if (y < height - 1 && grid[coordToIndex(x, y + 1)] == 0) // Down
            adjList[index].push_back({ coordToIndex(x, y + 1), weight });
    }

    pair<int, int> indexToCoord(int index) const {
        return { index % width, index / width };
    }

    int manhattanDistance(int index1, int index2) const {
        pair<int,int> coord1 = indexToCoord(index1);
        pair<int,int> coord2 = indexToCoord(index2);
        return abs(coord1.first - coord2.first) + abs(coord1.second - coord2.second);
    }

public:
    // Constructor to initialize the grid graph with dimensions and obstacles
    GridGraph(int w, int h, const std::vector<std::pair<int, int>>& obstacles) : width(w), height(h), adjList(w* h), grid(w* h, 0) {
        // Initialize obstacles in the grid
        for (const auto& obs : obstacles) {
            grid[coordToIndex(obs.first, obs.second)] = 1;
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
        int start = coordToIndex(startX, startY);
        int goal = coordToIndex(goalX, goalY);

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> frontier;
        std::vector<int> costSoFar(width * height, std::numeric_limits<int>::max());
        std::vector<int> cameFrom(width * height, -1);

        frontier.emplace(0, start);
        costSoFar[start] = 0;
        cameFrom[start] = start;

        while (!frontier.empty()) {
            int current = frontier.top().second;
            frontier.pop();

            if (current == goal) break;

            for (const auto& edge : adjList[current]) {
                int next = edge.first;
                int newCost = costSoFar[current] + edge.second;

                if (newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    int priority = newCost + manhattanDistance(next, goal);
                    frontier.emplace(priority, next);
                    cameFrom[next] = current;
                }
            }
        }

        // Reconstruct path
        std::vector<int> path;
        for (int at = goal; at != start; at = cameFrom[at]) {
            if (at == -1) return {}; // No path found
            path.push_back(at);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }



    std::vector<int> dijkstraSearch(int startX, int startY, int goalX, int goalY) {
        int start = coordToIndex(startX, startY);
        int goal = coordToIndex(goalX, goalY);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> priorityQueue;
        vector<int> dist(width * height, std::numeric_limits<int>::max());
        vector<int> prev(width * height, -1);

        dist[start] = 0;
        priorityQueue.emplace(0, start);

        while (!priorityQueue.empty()) {
            int current = priorityQueue.top().second;
            priorityQueue.pop();

            if (current == goal) break;

            for (const auto& edge : adjList[current]) {
                int next = edge.first;
                int weight = edge.second;
                int distanceThroughU = dist[current] + weight;

                if (distanceThroughU < dist[next]) {
                    dist[next] = distanceThroughU;
                    prev[next] = current;
                    priorityQueue.emplace(dist[next], next);
                }
            }
        }

        // Reconstruct path
        vector<int> path;
        for (int at = goal; at != -1; at = prev[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (path.front() != start) path.clear(); // No path found
        return path;
    }


    // Print the grid and the path from start to goal
    void printGrid(const vector<int>& path) {
        vector<char> gridDisplay(width * height, '.');
        for (int i = 0; i < grid.size(); ++i) {
            if (grid[i] == 1) {
                gridDisplay[i] = '#'; // Obstacle
            }
        }
        for (int idx : path) {
            gridDisplay[idx] = '*'; // Path
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cout << gridDisplay[coordToIndex(x, y)] << ' ';
            }
            cout << endl;
        }
    }
};

