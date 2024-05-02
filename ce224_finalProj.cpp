// 31 April 2024
// CE 224 
// HMWK 8 - Final Project
// g++ --std=c++11 ce224_finalProj.cpp
//./a.out
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <random>
#include <queue>
#include <climits>
#include <chrono>
#include <thread>
#include <algorithm> // for shuffle
#include <sstream> // for std::stringstream

using namespace std;

// Parent class for main menu/game selection
class gameMenu {
    public:
    int username; // initializing the name of the username , future work
    int lives; // initializing the number of lives used in each minigame , future work

    int menuSel(); // outputs a menu for the minigames to be selected and difficulty level
    void gameOver(int x); // outputs game over screen with try again, main menu, or exit screen
};

// outputs a menu for the minigames to be selected and difficulty level
int gameMenu::menuSel() {
    int choice;
    cout << "------------------- START GAME ------------------- \n";
    cout << "Please select a mini-game:\n";
    cout << "1. Maze Minigame\n";
    cout << "2. Bin Packing Minigame\n";
    cout << "3. Knapsack Algorithm Minigame\n";
    cout << "4. Minimum Spanning Trees Minigame\n";
    cout << "Enter your choice (1-4): ";

    // Validate and read game choice
    while (!(cin >> choice) || choice < 1 || choice > 4) {
        cout << "Invalid choice. Please enter a valid integer between 1 and 4: ";
        cin.clear();  // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
    }

    cin.ignore(); // Consume newline character

    return choice;
}

void gameMenu::gameOver(int x) {
    if (x == 0) {
        cout << "Game Over! Try again." << endl;
    } else if (x == 1) {
        cout << "Congratulations! You won!" << endl;
    } else {
        cout << "Invalid game status." << endl;
    }
}

// ------------------------------------------ MAZE MINIGAME ------------------------------------------
// Child class for game 1 - Maze minigame
class maze : public gameMenu {
public:
    map<int, array<int, 2>> map; // initialize edges map
    vector<int> s;               // initialize disjoint set vector
    vector<int> m;               // initialize maze vector

    void origMap(int x, int y, int z); // original map that defines edges
    int find(int x) const;
    void p(int x);                    // map disjoint set
    vector<int> makeMaze();           // create maze
    void renderMaze(const vector<int>& mazeEdges); // display maze
    bool mazeSolve(vector<int> solutionMaze); // user input solution
    vector<int> getUserSolution(); // Placeholder for getting user input
};

void maze::renderMaze(const vector<int>& mazeEdges) {
    const int rows = 5;
    const int cols = 5;

    // Initialize the maze grid with walls and cell numbering
    char mazeGrid[2 * rows + 1][7 * cols + 1]; // Adjusted width for two-digit cell numbers
    int cellNumber = 0;
    for (int i = 0; i < 2 * rows + 1; ++i) {
        for (int j = 0; j < 7 * cols + 1; ++j) {
            if (i % 2 == 0 ) {
                if (j % 7 == 0) {
                    mazeGrid[i][j] = '+'; // Corners
                } else {
                    mazeGrid[i][j] = '-'; // Horizontal walls
                }
            } else {
                if (j % 7 == 0) {
                    mazeGrid[i][j] = '|'; // Vertical walls
                } else {
                    mazeGrid[i][j] = ' '; // Passages
                    if (j % 7 == 3) { // Adjusted position for two-digit cell numbers
                        if (cellNumber < 10) {
                            mazeGrid[i][j] = to_string(cellNumber)[0]; // Cell numbering (tens digit)
                        } else {
                            mazeGrid[i][j] = to_string(cellNumber)[0]; // Cell numbering (tens digit)
                            mazeGrid[i][j + 1] = to_string(cellNumber)[1]; // Cell numbering (ones digit)
                            j += 1; // Increment j to skip the next position
                        }
                        cellNumber++;
                    }
                }
            }
        }
    }

    // Update the grid with corridors based on the maze edges
    for (int i = 0; i < mazeEdges.size(); ++i) {
        int edgeNum = mazeEdges[i];
        int row = 2 * (edgeNum / cols) + 1;
        int col = 7 * (edgeNum % cols) + 3; // Adjusted position for two-digit cell numbers

        mazeGrid[row][col] = ' '; // Mark passage
    }

    // Print the maze grid
    for (int i = 0; i < 2 * rows + 1; ++i) {
        for (int j = 0; j < 7 * cols + 1; ++j) {
            cout << mazeGrid[i][j];
        }
        cout << endl;
    }
}

void maze::origMap(int x, int y, int z) {
    map.insert(make_pair(x, array<int, 2>{{y, z}}));
}

int maze::find(int x) const {
    if (s[x] < 0) { // if the value of the element index is negative it is a root
        return x;
    } else {
        return find(s[x]); // recursion to get to the root
    }
}

void maze::p(int x) {
    x = x*x; // 5x5 maze dynamic 
    for (int i=0; i < x; i ++) {
        s.push_back(-1);
    }
}

// Function to create the maze
vector<int> maze::makeMaze() {
   // random number generator for 0-39
    vector<int> edges{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};
    random_device rd;
    mt19937 g(rd());
    shuffle(edges.begin(), edges.end(), g);
    // iterate through random values to check if they should be included in the map
    for (int i = 0; i < 40; i++) {
        int edgeNum = edges[i];
        auto it = map.find(edgeNum); // finding array values based on edge index generated
        if (it != map.end()) {
            array<int, 2>& arr = it->second;
            int x = arr[0];
            int y = arr[1];
            int root1 = find(x); // checking if the sets are already unionized
            int root2 = find(y);
            if (root1 != root2) { // union if sets are not unionized
                s[root2] = root1; // setting index value to the root
            }
            else {
                m.push_back(it->first); // adding edges that do not contribute to union to the maze
            }
        } 
    }
    // Printing the maze edges 
    cout <<"Maze Edges" <<  endl;
    for (int i=0; i < m.size(); i++) {
        auto it = map.begin();
        advance(it, m.at(i));
        cout << "(" << it->second[0] << ", " << it->second[1] << ")" << endl;
    }  
    cout << "\n";
    return m;
};

vector<int> maze::getUserSolution() {
    vector<int> userSolution;

    // Prompt the user to enter the path through the maze
    cout << "Enter the path through the maze (separate each cell number with a space): ";

    // Read the input from the user
    string input;
    getline(cin, input);

    // Parse the input string into integers representing the path
    istringstream iss(input);
    int cellNumber;
    while (iss >> cellNumber) {
        userSolution.push_back(cellNumber);
    }

    return userSolution;
}

bool maze::mazeSolve(vector<int> solutionMaze) {
    vector<int> visited(25, 0);
    queue<int> q;
    q.push(0);
    visited[0] = 1;
    int destination = 24;

    while (!q.empty()) {
        int front = q.front();
        q.pop();
        if (front == destination) {
            return true;
        }
        vector<int> neighbors = {front - 5, front + 5, front - 1, front + 1};
        for (int neighbor : neighbors) {
            if (neighbor >= 0 && neighbor < 25 && !visited[neighbor] && std::find(solutionMaze.begin(), solutionMaze.end(), neighbor) != solutionMaze.end()) {
                visited[neighbor] = 1;
                q.push(neighbor);
            }
        }
    }
    return false;
}

// ------------------------------------------ BIN PACKING ------------------------------------------
// Child class for game 2 - Bin Packing
class binPack : public gameMenu {
public:
    int bestFit(int weight[], int n, int c);// optimal function for the bin packing
    bool binSolve(int weight[], int n, int c); // user input solution
    bool isValidItem(const string& input, int n); // check if item number is valid
    bool allItemsUsed(const vector<vector<int>>& userBins, int n); // check if all items have been used
};

int binPack::bestFit(int weight[], int n, int c) {
    // Initialize result (Count of bins)
    int count = 0;

    // array to store remaining space in bins
    int remainingBin[n];

    // Place items one by one
    for (int i = 0; i < n; i++) {
        // Find the best fit bin
        int j;
        int min = c + 1, bi = 0;

        for (j = 0; j < count; j++) {
            if (remainingBin[j] >= weight[i] && remainingBin[j] -  weight[i] < min) {
                bi = j;
                min = remainingBin[j] - weight[i];
            }
        }

        // create a new bin
        if (min == c + 1) {
            remainingBin[count] = c - weight[i];
            count++;
        }
        else // Assign the item to best bin
            remainingBin[bi] -= weight[i];
    }
    return count;
}

// user input solution
bool binPack::binSolve(int weight[], int n, int c) {
    // Calculate the correct bins using the bestFit function
    int correctBins = bestFit(weight, n, c);

    vector<vector<int>> userBins(correctBins);

    cout << "\nInstructions: Enter the item numbers to place in each bin. Separate item numbers by spaces.\n";

    // Clear the input stream
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Prompt the user for their bin configuration one bin at a time
    for (int i = 0; i < correctBins; ++i) {
        cout << "Bin " << i + 1 << ": ";
        string input;
        getline(cin, input);
        stringstream ss(input);
        int item, totalWeight = 0;
        bool validInput = true;
        while (ss >> item) {
            if (!isValidItem(input, n)) {
                cout << "Invalid input. Please enter valid item numbers for Bin " << i + 1 << "." << endl;
                validInput = false;
                break;
            }
            // Check if the item is already in another bin
            bool alreadyInBin = false;
            for (int j = 0; j < i; ++j) {
                if (find(userBins[j].begin(), userBins[j].end(), item) != userBins[j].end()) {
                    alreadyInBin = true;
                    break;
                }
            }
            if (alreadyInBin) {
                cout << "Item " << item << " is already in another bin. Please choose a different item." << endl;
            } else {
                userBins[i].push_back(item);
                totalWeight += weight[item - 1]; // Subtract 1 because item numbers start from 1
            }
        }
        // Check if the total weight in the bin exceeds the maximum capacity
        if (validInput && totalWeight > c) {
            cout << "The total weight in Bin " << i + 1 << " exceeds the maximum capacity of " << c << " lbs. You lose!" << endl;
            gameOver(0);
            return false;
        }
        if (!validInput) {
            // Repeat the same bin iteration if input is invalid
            i--;
        }

        // Check if the user wins before prompting for the next bin
        if (userBins.size() < correctBins) {
            cout << "\nCongratulations! You won! You used fewer bins than the algorithm.\n";
            gameOver(1);
            return true;
        } else if (allItemsUsed(userBins, n)) {
            cout << "\n Bins emptied the item list in fewer bins than the algorithm!\n";
            gameOver(1);
            return true;
        }
    }

    // If the loop completes without the user winning, they lose
    cout << "\nIncorrect! You used the same number of bins as the algorithm or more.\n";
    gameOver(0);
    return false;
}


bool binPack::isValidItem(const string& input, int n) {
    stringstream ss(input);
    int item;
    while (ss >> item) {
        if (item < 1 || item > n) {
            return false;
        }
    }
    return true;
}

bool binPack::allItemsUsed(const vector<vector<int>>& userBins, int n) {
    vector<int> itemsUsed;
    for (const auto& bin : userBins) {
        itemsUsed.insert(itemsUsed.end(), bin.begin(), bin.end());
    }
    sort(itemsUsed.begin(), itemsUsed.end());
    vector<int> allItems(n);
    for (int i = 1; i <= n; ++i) {
        allItems[i - 1] = i;
    }
    return itemsUsed == allItems;
}

// ------------------------------------------ KNAPSACK ALGORITHM ------------------------------------------
// Child class for game 3 - Knapsack Algorithm
class knapSack : public gameMenu {
private:
    int max(int a, int b) { return (a > b) ? a : b; } // Utility function
public:
    int knapSackAlgo(int W, int wt[], int val[], int n); // optimal knapsack algorithm
    bool knapSolve(int W, int wt[], int val[], int n); // user input solution
};


// Knapsack solver algorithm function
int knapSack::knapSackAlgo(int W, int wt[], int val[], int n) {
    // Base Case
    if (n == 0 || W == 0) {
        return 0;
    }

    if (wt[n - 1] > W) {
        return knapSackAlgo(W, wt, val, n - 1);
    }
    else {
        return max(
            val[n - 1] + knapSackAlgo(W - wt[n - 1], wt, val, n - 1), knapSackAlgo(W, wt, val, n - 1));
    }
}

// user input solution function
bool knapSack::knapSolve(int W, int wt[], int val[], int n) {
    // Calculate the correct solution using knapSackAlgo
    int correctSolution = knapSackAlgo(W, wt, val, n);

    // Prompt the user for their solution
    int userSolution;
    cout << "\nInstructions - You have a knapsack with a maximum capacity of " << W << " units.\n";
    cout << "You are given " << n << " items with their weights and values.\n";
    cout << "Items and their weights:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Item " << i + 1 << ": Weight = " << wt[i] << ", Value = " << val[i] << endl;
    }
    cout << "Enter the maximum value that can be obtained by selecting some items to put in the knapsack: ";
    cin >> userSolution;

    // Compare the user's solution with the correct solution
    if (userSolution == correctSolution) {
        gameOver(1); // Call gameOver with success code

        return true;
    }
    else {
        gameOver(0); // Call gameOver with failure code
        return false;
    }
};

// ------------------------------------------ MST MINIGAME ------------------------------------------
// Edge class for MST minigame
class Edge {
public:
    int u, v;
    int weight;
    // constructor
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
};

// EdgeType class for MST minigame
class EdgeType {
public:
    int u, v;
    int weight;
    // constructor
    EdgeType(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    // PQ comparison operator
    bool operator<(const EdgeType& other) const {
        return weight > other.weight; // min-heap
    }
};

// Disjoint Set Tree Representation for MST minigame
class DisjointSetsMST {
private:
    vector<int> djSet;

public:
    DisjointSetsMST(int n) {
        for (int i = 0; i < n; i++) {
            djSet.push_back(-1);
        }
    }
    // Functions
    int find(int x) const;
    void unionS(int x, int y);
};

// Disjoint Set Tree find function for MST minigame
int DisjointSetsMST::find(int x) const {
    if (djSet[x] < 0) { // if the value of the element index is negative it is a root
        return x;
    } else {
        return find(djSet[x]); // recursion to get to the root
    }
}

// Disjoint Set Tree union function for MST minigame
void DisjointSetsMST::unionS(int x, int y) {
    int root1 = find(x);
    int root2 = find(y);
    if (root1 != root2) {
        if (djSet[root1] < djSet[root2]) {
            djSet[root1] += djSet[root2];
            djSet[root2] = root1;
        } else {
            djSet[root2] += djSet[root1];
            djSet[root1] = root2;
        }
    }
}

// Child class for game 4 - Minimum Spanning Trees
class MST : public gameMenu {
private:
    vector<int> djSet;

public:
    MST(int n) {
        for (int i = 0; i < n; i++) {
            djSet.push_back(-1);
        }
    }
    vector<EdgeType> createGraph(); // create graph
    vector<Edge> kruskal(vector<EdgeType> edges); // kruskal algorithm
    bool mstSolve(int userWeight, int correctWeight); // user input solution
};

// Function to create a graph for MST minigame
vector<EdgeType> MST::createGraph() {
    vector<EdgeType> edges;

    // Example graph generation based on difficulty
    edges.push_back(EdgeType(0, 1, 4));
    edges.push_back(EdgeType(0, 7, 8));
    edges.push_back(EdgeType(1, 2, 8));
    edges.push_back(EdgeType(1, 7, 11));
    edges.push_back(EdgeType(2, 3, 7));
    edges.push_back(EdgeType(2, 5, 4));
    edges.push_back(EdgeType(2, 8, 2));
    edges.push_back(EdgeType(3, 4, 9));
    edges.push_back(EdgeType(3, 5, 14));
    edges.push_back(EdgeType(4, 5, 10));
    edges.push_back(EdgeType(5, 6, 2));
    edges.push_back(EdgeType(6, 7, 1));
    edges.push_back(EdgeType(6, 8, 6));
    edges.push_back(EdgeType(7, 8, 7));

    return edges;
}

// Function to perform Kruskal's algorithm for MST minigame
vector<Edge> MST::kruskal(vector<EdgeType> edges) {
    vector<Edge> result;
    priority_queue<EdgeType> pq;

    // Push all the edges to the priority queue
    for (auto e : edges) {
        pq.push(e);
    }

    // Create disjoint sets
    DisjointSetsMST ds(edges.size());

    while (!pq.empty() && result.size() != edges.size() - 1) {
        // Pick the smallest edge
        EdgeType e = pq.top();
        pq.pop();

        int x = ds.find(e.u);
        int y = ds.find(e.v);

        // If including this edge doesn't cause cycle, include it in result and union the sets
        if (x != y) {
            result.push_back(Edge(e.u, e.v, e.weight));
            ds.unionS(x, y);
        }
    }

    return result;
}

bool MST::mstSolve(int userWeight, int correctWeight) {
    if (userWeight == correctWeight) {
        gameOver(1); // Call gameOver with appropriate parameter
        return true;
    } else {
        gameOver(0); // Call gameOver with appropriate parameter
        cout << "Correct MST weight: " << correctWeight << endl;
        return false;
    }
}

// ------------------------------------------ MAIN FUNCTION ------------------------------------------
int main() {
    // Main menu
    gameMenu menu;
    int gameChoice = menu.menuSel();

    
    if (gameChoice == 1) { // Maze minigame
        maze mazeGame;
        cout << "---------- MAZE MINIGAME ----------" << endl;
        maze maze;
        maze.origMap(0,0,1);
        maze.origMap(1,1,2);
        maze.origMap(2,2,3);
        maze.origMap(3,3,4);
        maze.origMap(4,0,5);
        maze.origMap(5,1,6);
        maze.origMap(6,2,7);
        maze.origMap(7,3,8);
        maze.origMap(8,4,9);
        maze.origMap(9,5,6);
        maze.origMap(10,6,7);
        maze.origMap(11,7,8);
        maze.origMap(12,8,9);
        maze.origMap(13,5,10);
        maze.origMap(14,6,11);
        maze.origMap(15,7,12);
        maze.origMap(16,8,13);
        maze.origMap(17,9,14);
        maze.origMap(18,10,11);
        maze.origMap(19,11,12);
        maze.origMap(20,12,13);
        maze.origMap(21,13,14);
        maze.origMap(22,10,15);
        maze.origMap(23,11,16);
        maze.origMap(24,12,17);
        maze.origMap(25,13,18);
        maze.origMap(26,14,19);
        maze.origMap(27,15,16);
        maze.origMap(28,16,17);
        maze.origMap(29,17,18);
        maze.origMap(30,18,19);
        maze.origMap(31,20,15);
        maze.origMap(32,21,16);
        maze.origMap(33,17,22);
        maze.origMap(34,18,23);
        maze.origMap(35,19,24);
        maze.origMap(36,20,21);
        maze.origMap(37,21,22);
        maze.origMap(38,22,23);
        maze.origMap(39,23,24);
        maze.p(5);
        maze.makeMaze();

        vector<int> mazeEdges = mazeGame.makeMaze();
        mazeGame.renderMaze(mazeEdges);
        cout << "Solve the maze!" << endl;

        // Get user input for solving the maze
        vector<int> userSolution;
        userSolution = mazeGame.getUserSolution();

        bool solutionCorrect = mazeGame.mazeSolve(userSolution); // Check if the solution is correct
        if (solutionCorrect) {
            menu.gameOver(1); // Game won
        } else {
            menu.gameOver(0); // Game lost
        }
    }   


   // Game 2: Bin Packing
    if (gameChoice == 2) {
        cout << "\n\n ---------- BIN PACKING MINIGAME ----------" << endl;
        int weight[] = {2, 5, 4, 7, 1, 3, 8};
        int n = sizeof(weight) / sizeof(weight[0]);
        int c = 10; // Bin capacity
        // Display available items (optional)
        binPack level1BP;
        int correctBins = level1BP.bestFit(weight, n, c);
        cout << "Instructions: Try to use less bins than the algorithm. The algorithm used " << correctBins << " bins." << endl;
        cout << "Each bin can hold " << c << "lbs.";
        cout << "\nAvailable items (weights): ";
        for (int i = 0; i < n; ++i) {
            cout << "Item " << i+1 << ":" << weight[i] << "lbs, ";
        }
        cout << endl;

        // Example usage of bin packing minigame
        binPack myBinPack;
        myBinPack.binSolve(weight, n, c); // Solve bin packing minigame
    }

    // Game 3: Knapsack Algorithm
    if (gameChoice == 3) {
        cout << "\n\n ---------- KNAPSACK MINIGAME ----------" << endl;
        // Example usage of Knapsack Algorithm minigame
        knapSack myKnapSack;
        int val[] = {60, 100, 120};
        int wt[] = {10, 20, 30};
        int W = 50; // Knapsack capacity
        myKnapSack.knapSolve(W, wt, val, sizeof(val) / sizeof(val[0]));
    }

    // Game 4: Minimum Spanning Trees
    if (gameChoice == 4) {
        cout << "\n\n ---------- MINIMUM SPANNING TREE MINIGAME ----------" << endl;

        // Example usage of Minimum Spanning Trees minigame
        MST myMST(9);                                 // 9 is the number of vertices in the graph
        vector<EdgeType> edges = myMST.createGraph(); // Create a graph for difficulty level 1
        vector<Edge> solution = myMST.kruskal(edges); // Example usage of Kruskal's algorithm

        // Display the graph
        cout << "Graph:" << endl;
        for (const auto& edge : edges) {
            cout << edge.u << " - " << edge.v << " (Weight: " << edge.weight << ")" << endl;
        }

        // Input user's solution
        cout << "Instructions - Enter the total weight of your MST: ";
        int userWeight;
        cin >> userWeight;

        int correctWeight = 0;
        for (const auto& edge : solution) {
            correctWeight += edge.weight;
        }

        myMST.mstSolve(userWeight, correctWeight); // Check user's solution
    }

    return 0;
}
