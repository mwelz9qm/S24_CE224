#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int u) {
        if (parent[u] != u)
            parent[u] = find(parent[u]);
        return parent[u];
    }

    void merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u != v) {
            if (rank[u] < rank[v])
                swap(u, v);
            parent[v] = u;
            if (rank[u] == rank[v])
                rank[u]++;
        }
    }
};

void generateMaze(char**& maze, int height, int width);
void deleteMaze(char**& maze, int height);
void printMaze(char** maze, int height, int width);
void printMazeWithRoute(char** maze, int height, int width, const vector<pair<int, int>>& optimalRoute);
bool playerAction(char** maze, int& posX, int& posY, int& lives, vector<string>& inventory);
void moveEnemies(char** maze, int height, int width, int& posX, int& posY, int& lives);
void generateChests(char** maze, int height, int width);
void ensureExit(char** maze, int height, int width);
void ensureKey(char** maze, int height, int width);


vector<pair<int, int>> calculateOptimalRoute(char** maze, int height, int width, int posX, int posY, vector<string>& inventory);

int main()
{
    int height, width;
    cout << "Enter height of the maze: ";
    cin >> height;
    cout << "Enter width of the maze: ";
    cin >> width;

    char** maze;
    generateMaze(maze, height, width);

    int posX = 1, posY = 1;
    int lives = 3; // Player's lives

    vector<string> inventory;

    bool quit = false;
    while (!quit && lives > 0)
    {
        system("cls");
        cout << "Control your player using WASD. Press Q to quit." << endl;
        cout << "Lives: " << lives << endl;
        cout << "Inventory: ";
        for (const auto& item : inventory)
        {
            cout << item << " ";
        }
        cout << endl << endl;
        maze[posX][posY] = 'P'; // Player position

        moveEnemies(maze, height, width, posX, posY, lives);

        printMaze(maze, height, width);

        quit = playerAction(maze, posX, posY, lives, inventory);
    }

    if (lives <= 0)
    {
        cout << "Game Over! You ran out of lives." << endl;
        cout << "Calculating the most optimal route to the nearest chest containing a key..." << endl;

        vector<pair<int, int>> optimalRoute = calculateOptimalRoute(maze, height, width, posX, posY, inventory);
        if (optimalRoute.empty())
        {
            cout << "No valid path found to a chest containing a key. Game over." << endl;
        }
        else
        {
            cout << "Optimal route to the nearest chest containing a key:" << endl;
            // Print the maze with the optimal route marked with 'X'
            printMazeWithRoute(maze, height, width, optimalRoute);
        }
    }

    deleteMaze(maze, height);

    return 0;
}
void generateMaze(char**& maze, int height, int width)
{
    maze = new char* [height];
    for (int i = 0; i < height; ++i)
    {
        maze[i] = new char[width];
        for (int j = 0; j < width; ++j)
        {
            // Initialize maze with walls on the border and empty spaces inside
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                maze[i][j] = '#'; // Wall
            }
            else
            {
                maze[i][j] = ' '; // Empty space
            }
        }
    }

    // Randomly generate walls inside the maze
    srand(time(NULL)); // Seed the random number generator
    int numWalls = height * width / 4; // Adjust this number as needed
    for (int k = 0; k < numWalls; ++k)
    {
        int randX = 1 + rand() % (height - 2); // Generate random x coordinate
        int randY = 1 + rand() % (width - 2); // Generate random y coordinate
        maze[randX][randY] = '#'; // Place a wall at the random positions
    }

    // Randomly generate enemies inside the maze represented by '?'
    int numEnemies = height * width / 20; // Adjust this number as needed
    for (int k = 0; k < numEnemies; ++k)
    {
        int randX = 1 + rand() % (height - 2); // Generates random x coordinate
        int randY = 1 + rand() % (width - 2); // Generates random y coordinate
        if (maze[randX][randY] != '#' && maze[randX][randY] != 'P') // Make sure the position is not a wall or player
        {
            maze[randX][randY] = '?'; // Place an enemy at the random position
        }
    }

    generateChests(maze, height, width);
    ensureExit(maze, height, width);
    ensureKey(maze, height, width);
}

void deleteMaze(char**& maze, int height)
{
    for (int i = 0; i < height; ++i)
    {
        delete[] maze[i];
    }
    delete[] maze;
}

void printMaze(char** maze, int height, int width)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

void printMazeWithRoute(char** maze, int height, int width, const vector<pair<int, int>>& optimalRoute)
{
    // Create a copy of the maze to preserve the original maze
    char** mazeCopy = new char*[height];
    for (int i = 0; i < height; ++i)
    {
        mazeCopy[i] = new char[width];
        for (int j = 0; j < width; ++j)
        {
            mazeCopy[i][j] = maze[i][j];
        }
    }

    // Mark the optimal route with 'X' in the maze copy
    for (const auto& pos : optimalRoute)
    {
        int x = pos.first;
        int y = pos.second;
        if (mazeCopy[x][y] != 'P' && mazeCopy[x][y] != 'E' && mazeCopy[x][y] != 'D' && mazeCopy[x][y] != 'C')
        {
            mazeCopy[x][y] = 'X';
        }
    }

    // Print the maze with the optimal route marked
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            cout << mazeCopy[i][j];
        }
        cout << endl;
    }

    // Free the memory allocated for the maze copy
    for (int i = 0; i < height; ++i)
    {
        delete[] mazeCopy[i];
    }
    delete[] mazeCopy;
}
bool playerAction(char** maze, int& posX, int& posY, int& lives, vector<string>& inventory)
{
    cout << "\nAction: ";
    char action;
    cin >> action;

    int prevPosX = posX;
    int prevPosY = posY;

    switch (action)
    {
    case 'a':
        if (maze[posX][posY - 1] != '#' && !(maze[posX][posY - 1] == 'D' && find(inventory.begin(), inventory.end(), "Key") == inventory.end()))
        {
            posY--;
        }
        break;
    case 'd':
        if (maze[posX][posY + 1] != '#' && !(maze[posX][posY + 1] == 'D' && find(inventory.begin(), inventory.end(), "Key") == inventory.end()))
        {
            posY++;
        }
        break;
    case 'w':
        if (maze[posX - 1][posY] != '#' && !(maze[posX - 1][posY] == 'D' && find(inventory.begin(), inventory.end(), "Key") == inventory.end()))
        {
            posX--;
        }
        break;
    case 's':
        if (maze[posX + 1][posY] != '#' && !(maze[posX + 1][posY] == 'D' && find(inventory.begin(), inventory.end(), "Key") == inventory.end()))
        {
            posX++;
        }
        break;
    case 'Q':
    case 'q':
        cout << "Quitting the maze game..." << endl;
        return true; // End the loop by indicating to quit
    default:
        cout << "Invalid action!" << endl;
        break;
    }

    if (maze[posX][posY] == '?')
    {
        lives--; // Decrement lives when player hits an enemy
        cout << "You lost a life! Lives remaining: " << lives << endl;
    }
    else if (maze[posX][posY] == 'C')
    {
        // Player found a chest, check its contents
        int randChance = rand() % 2; // Random chance for contents of the chest
        if (randChance == 0)
        {
            cout << "You found an empty chest." << endl;
        }
        else
        {
            inventory.push_back("Key");
            cout << "You found a chest with a key!" << endl;
        }
        maze[posX][posY] = ' '; // Removes the chest from the maze
    }
    else if (maze[posX][posY] == 'E')
    {
        // Player reached the exit
        if (find(inventory.begin(), inventory.end(), "Key") != inventory.end())
        {
            cout << "Congratulations! You beat the maze!" << endl;
            // Remove the key from the inventory since it's used to unlock the door
            inventory.erase(remove(inventory.begin(), inventory.end(), "Key"), inventory.end());
            return true; // Ends the loop by reaching the exit
        }
        else
        {
            cout << "You need a key to open the exit!" << endl;
            posX = prevPosX;
            posY = prevPosY;
        }
    }

    maze[prevPosX][prevPosY] = ' '; // Clears the previous position

    return false; // Continues the loop
}

void moveEnemies(char** maze, int height, int width, int& posX, int& posY, int& lives)
{
    // Find all enemy positions and move them randomly
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (maze[i][j] == '?')
            {
                // Generate random direction
                int dir = rand() % 4; // 0: up, 1: down, 2: left, 3: right
                int newI = i, newJ = j;
                switch (dir)
                {
                case 0:
                    newI--;
                    break;
                case 1:
                    newI++;
                    break;
                case 2:
                    newJ--;
                    break;
                case 3:
                    newJ++;
                    break;
                }

                // Check if the new position is valid and not a wall, player, exit, or chest
                if (newI >= 0 && newI < height && newJ >= 0 && newJ < width &&
                    maze[newI][newJ] != '#' && maze[newI][newJ] != 'P' && maze[newI][newJ] != 'E' && maze[newI][newJ] != 'C')
                {
                    // Move the enemy to the new position
                    if (newI == posX && newJ == posY)
                    {
                        // Enemy caught the player, decrement lives
                        lives--;
                        cout << "You lost a life! Lives remaining: " << lives << endl;
                        if (lives <= 0)
                        {
                            return; // End the game if no lives remaining
                        }
                    }
                    maze[i][j] = ' '; // Clear previous position
                    maze[newI][newJ] = '?'; // Move enemy to new position
                }
            }
        }
    }
}

vector<pair<int, int>> calculateOptimalRoute(char** maze, int height, int width, int posX, int posY, vector<string>& inventory)
{
    // Initialize visited array to keep track of visited positions
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    // Initialize queue for breadth-first search
    queue<pair<int, int>> q;

    // Start from player's position
    q.push({ posX, posY });
    visited[posX][posY] = true;

    // Initialize parent array to reconstruct the path
    vector<vector<pair<int, int>>> parent(height, vector<pair<int, int>>(width, { -1, -1 }));

    // Perform breadth-first search
    while (!q.empty())
    {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Check if the current position contains a chest with a key
        if (maze[x][y] == 'C' && find(inventory.begin(), inventory.end(), "Key") == inventory.end())
        {
            // Reconstruct the path from the player's position to this chest
            vector<pair<int, int>> path;
            while (x != posX || y != posY)
            {
                path.push_back({ x, y });
                int tempX = parent[x][y].first;
                int tempY = parent[x][y].second;
                x = tempX;
                y = tempY;
            }
            path.push_back({ posX, posY });
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore all adjacent positions
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (const auto& dir : directions)
        {
            int newX = x + dir.first;
            int newY = y + dir.second;

            // Check if the new position is within bounds and not a wall or visited
            if (newX >= 0 && newX < height && newY >= 0 && newY < width &&
                maze[newX][newY] != '#' && !visited[newX][newY])
            {
                q.push({ newX, newY });
                visited[newX][newY] = true;
                parent[newX][newY] = { x, y };
            }
        }
    }

    // No path found
    return {};
}

void generateChests(char** maze, int height, int width)
{
    // Randomly generate chests inside the maze represented by 'C'
    int numChests = height * width / 50; // Adjust this number as needed
    int keyIndex = rand() % numChests; // Random index for the chest containing the key
    for (int k = 0; k < numChests; ++k)
    {
        int randX = 1 + rand() % (height - 2); // Generate random x coordinate
        int randY = 1 + rand() % (width - 2); // Generate random y coordinate
        if (maze[randX][randY] != '#' && maze[randX][randY] != 'P' && maze[randX][randY] != 'E') // Make sure the position is not a wall, player, or exit
        {
            if (k == keyIndex)
            {
                maze[randX][randY] = 'C'; // Place a chest containing a key at the random position
            }
            else
            {
                maze[randX][randY] = 'C'; // Place a regular chest at the random position
            }
        }
    }
}

void ensureExit(char** maze, int height, int width)
{
    int exitX = height - 2; // Bottom row
    int exitY = width - 2; // Rightmost column

    // Place walls around the exit
    maze[exitX][exitY] = '#'; // Bottom right corner
    maze[exitX - 1][exitY] = '#'; // Above the exit
    maze[exitX + 1][exitY] = '#'; // Below the exit
    maze[exitX][exitY + 1] = '#'; // Right of the exit

    // Place the exit at the bottom right corner
    maze[exitX][exitY] = 'E';

    // Place the door (D) to the left of the exit
    maze[exitX][exitY - 1] = 'D';
}

void ensureKey(char** maze, int height, int width)
{
    // Ensure at least one key in smaller mazes
    if (height < 10 || width < 10)
    {
        int randX = 1 + rand() % (height - 2); // Generate random x coordinate
        int randY = 1 + rand() % (width - 2); // Generate random y coordinate
        maze[randX][randY] = 'C'; // Place a chest containing a key at the random position
    }
}
