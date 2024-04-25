#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

void generateMaze(char**& maze, int height, int width);
void deleteMaze(char**& maze, int height);
void printMaze(char** maze, int height, int width);
bool playerAction(char** maze, int& posX, int& posY, int& lives, vector<string>& inventory);
void moveEnemies(char** maze, int height, int width, int& posX, int& posY, int& lives);
void generateChests(char** maze, int height, int width);
void ensureExit(char** maze, int height, int width);
void ensureKey(char** maze, int height, int width);

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

    deleteMaze(maze, height);

    if (lives <= 0)
    {
        cout << "Game Over!" << endl;
    }

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
        maze[randX][randY] = '#'; // Place a wall at the random position
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
        if (maze[posX][posY - 1] != '#')
        {
            posY--;
        }
        break;
    case 'd':
        if (maze[posX][posY + 1] != '#')
        {
            posY++;
        }
        break;
    case 'w':
        if (maze[posX - 1][posY] != '#')
        {
            posX--;
        }
        break;
    case 's':
        if (maze[posX + 1][posY] != '#')
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
    // Ensure the exit is not blocked by walls
    int randX, randY;
    do
    {
        randX = 1 + rand() % (height - 2);
        randY = 1 + rand() % (width - 2);
    } while (maze[randX][randY] == '#' || maze[randX - 1][randY] == '#' || maze[randX + 1][randY] == '#' || maze[randX][randY - 1] == '#' || maze[randX][randY + 1] == '#');

    maze[randX][randY] = 'E'; // Places the exit at the random position
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