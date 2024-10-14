#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
//#include <unistd.h> // for sleep in Linux. Use <windows.h> and Sleep(ms) for Windows
#include <windows.h>

using namespace std;

class GameOfLife {
private:
    int rows, cols;
    vector<vector<int>> grid, tempGrid;

    // Helper to check if a cell is valid
    bool isValid(int x, int y) {
        return (x >= 0 && x < rows && y >= 0 && y < cols);
    }

    // Count neighbors that are alive
    int countAliveNeighbors(int x, int y) {
        int aliveNeighbors = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (!(i == 0 && j == 0) && isValid(x + i, y + j)) {
                    aliveNeighbors += grid[x + i][y + j];
                }
            }
        }
        return aliveNeighbors;
    }

    // Update the grid for the next step
    void updateGrid() {
        tempGrid = grid; // Copy current state

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);

                // Apply Conway's rules
                if (grid[i][j] == 1) {
                    // Cell is alive
                    if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                        tempGrid[i][j] = 0; // Dies
                    }
                }
                else {
                    // Cell is dead
                    if (aliveNeighbors == 3) {
                        tempGrid[i][j] = 1; // Comes to life
                    }
                }
            }
        }

        grid = tempGrid; // Move to the new state
    }

    // Display the grid
    void displayGrid() {
        cout << "\033[H";
        //int stepCount = 1;
        //system("CLS"); // Clears the console (use "CLS" for Windows)
        cout << "Step " << stepCount << ": " << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols - 1; ++j) {
                cout << ". "; // Show grid boundaries
                cout << (grid[i][j] ? 'O' : ' ') << " "; // O for alive, space for dead
            }
            cout << ".\n"; // Show grid boundaries
        }
        stepCount = stepCount + 1;
        //for (int j = 0; j <= cols; ++j)
            //cout << ".   "; // Bottom row grid boundary
        //cout << "\n";
    }

public:

    int stepCount = 1;

    // Constructor
    GameOfLife(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<int>(cols, 0));
        tempGrid.resize(rows, vector<int>(cols, 0));
    }

    // Initialize the grid with random alive cells
    void initializeGrid(int aliveCells) {
        srand(time(0)); // Seed for random number generation
        for (int i = 0; i < aliveCells; ++i) {
            int x = rand() % rows;
            int y = rand() % cols;
            grid[x][y] = 1;
        }
    }

    // Run the game for a given number of steps
    void run(int steps) {
        for (int i = 0; i < steps; ++i) {
            displayGrid();
            updateGrid();
            Sleep(200);
        }
    }

    void saveGame(const string& fileName) {
        ofstream file(fileName);
        if (file.is_open()) {
            file << rows << " " << cols << "\n";
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    file << grid[i][j] << " ";
                }
                file << "\n";
            }
            file.close();
            cout << "Game has been saved to " << fileName << "\n";
        }
        else {
            cerr << "Unable to save game. \n";
        }
    }

    bool loadGame(const string& fileName) {
        ifstream file(fileName);
        if (file.is_open()) {
            file >> rows >> cols;
            grid.resize(rows, vector<int>(cols, 0));
            tempGrid.resize(rows, vector<int>(cols, 0));
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    file >> grid[i][j];
                }
            }
            file.close();
            cout << "Game loaded from " << fileName << "\n";
            return true;
        }
        else {
            cerr << "Error: Unable to load the game.\n";
            return false;
        }
    }
};

template <typename T>
T getValidatedInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer value.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    int rows, cols, aliveCells, steps;

    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the number of columns: ";
    cin >> cols;
    cout << "Enter the number of initial alive cells: ";
    cin >> aliveCells;
    cout << "Enter the number of steps: ";
    cin >> steps;

    GameOfLife game(rows, cols);
    game.initializeGrid(aliveCells);
    game.run(steps);

    return 0;
}
