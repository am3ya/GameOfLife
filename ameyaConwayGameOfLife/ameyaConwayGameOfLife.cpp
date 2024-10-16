#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <conio.h>
#include <thread>
//#include <unistd.h> // for sleep in Linux. Use <windows.h> and Sleep(ms) for Windows
#include <windows.h>

using namespace std;

class GameOfLife {
private:
    int rows, cols;
    vector<vector<int>> grid, tempGrid, initialGrid;
    bool blockFound;
    bool horizontalBeehiveFound;

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

    // Detect if there's a block
    /*bool isBlockAt(int x, int y) {
        // Check if (x, y) is the top-left corner of a 2x2 block
        if (isValid(x, y) && isValid(x + 1, y) && isValid(x, y + 1) && isValid(x + 1, y + 1)) {
            if (grid[x][y] == 1 && grid[x + 1][y] == 1 && grid[x][y + 1] == 1 && grid[x + 1][y + 1] == 1) {
                // Now check surrounding neighbors
                for (int i = -1; i <= 2; ++i) {
                    for (int j = -1; j <= 2; ++j) {
                        if (!(i == 0 && j == 0) && !(i == 1 && j == 1) && isValid(x + i, y + j)) {
                            if (grid[x + i][y + j] == 1) {
                                return false; // There's an alive neighbor around the block
                            }
                        }
                    }
                }
                return true; // Found a block
            }
        }
        return false;
    }*/

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

    bool checkForBlocks() {
        //int blockStep;
        for (int i = 0; i < rows - 1; ++i) {
            for (int j = 0; j < cols - 1; ++j) {
                if (grid[i][j] == 1 && grid[i + 1][j] == 1 && grid[i][j + 1] == 1 && grid[i + 1][j + 1] == 1) {

                    if (countAliveNeighbors(i, j) == 3 && countAliveNeighbors(i+1, j) == 3 && countAliveNeighbors(i,j+1) == 3 && countAliveNeighbors(i+1,j+1) == 3) {
                        cout << "\nTop left of the block is " << i << ", " << j << endl;
                        blockFound = true;
                        return true;
                        //cout << "\nTop left of the block is " << i << ", " << j << endl;
                    }

                    /*if (isValid(i - 1, j - 1)) {
                        if (grid[i - 1][j - 1] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i - 1, j)) {
                        if (grid[i - 1][j] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i - 1, j + 1)) {
                        if (grid[i - 1][j + 1] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i - 1, j + 2)) {
                        if (grid[i - 1][j + 2] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i, j + 2)) {
                        if (grid[i][j + 2] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 1, j + 2)) {
                        if (grid[i + 1][j + 2] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 2, j + 2)) {
                        if (grid[i + 2][j + 2] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 2, j + 1)) {
                        if (grid[i + 2][j + 1] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 2, j)) {
                        if (grid[i + 2][j] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 2, j - 1)) {
                        if (grid[i + 2][j - 1] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i + 1, j - 1)) {
                        if (grid[i + 1][j - 1] == 1) {
                            return false;
                        }
                    }

                    if (isValid(i , j - 1)) {
                        if (grid[i][j - 1] == 1) {
                            return false;
                        }
                    }

                    blockFound = true;
                    return true;*/


                    
                    /*if (grid[i - 1][j - 1] == 0 && grid[i - 1][j] == 0 && grid[i - 1][j + 1] == 0 && grid[i - 1][j + 2] == 0
                        && grid[i][j + 2] == 0 && grid[i + 1][j + 2] == 0 && grid[i + 2][j + 2] == 0 && grid[i + 2][j + 1] == 0
                        && grid[i + 2][j] == 0 && grid[i + 2][j - 1] == 0 && grid[i + 1][j - 1] == 0 && grid[i][j - 1] == 0) {
                        //blockStep = stepCount;
                        //cout << "\nA block has been formed" << endl;
                        return true;
                    }*/
                }
            }
        }
        //return false;
        //cout << "No blocks found. \n";
    }


    bool checkForHorizontalBeehive() {
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 0; j < 27; ++j) {
                if (grid[i][j] == 1 && grid[i - 1][j + 1] == 1 && grid[i - 1][j + 2] == 1
                    && grid[i][j + 3] == 1 && grid[i + 1][j + 2] == 1 && grid[i + 1][j + 1] == 1) {


                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i - 1, j + 1) == 2)
                        && (countAliveNeighbors(i - 1, j + 2) == 2) && (countAliveNeighbors(i, j + 3) == 2)
                        && (countAliveNeighbors(i + 1, j + 2) == 2) && (countAliveNeighbors(i + 1, j + 1) == 2)) {

                        cout << endl;
                        cout << "\nLeft cell of the horizontal beehive is " << i << ", " << j << endl;
                        horizontalBeehiveFound = true;
                        return true;
                    }

                }
            }
        }
    }



    // Check for blocks and notify the user
    /*void checkForBlocks() {
        bool blockFound = false;
        for (int i = 0; i < rows - 1; ++i) {
            for (int j = 0; j < cols - 1; ++j) {
                if (isBlockAt(i, j)) {
                    cout << "Block found at position (" << i << ", " << j << ")\n";
                    blockFound = true;
                }
            }
        }
        if (!blockFound) {
            cout << "No blocks found.\n";
        }
    }*/

public:

    int stepCount = 1;

    // Constructor
    GameOfLife(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<int>(cols, 0));
        tempGrid.resize(rows, vector<int>(cols, 0));
    }

    // Initialize the grid with random alive cells
    /*void initializeGrid(int aliveCells) {
        srand(time(0)); // Seed for random number generation
        for (int i = 0; i < aliveCells; ++i) {
            int x = rand() % rows;
            int y = rand() % cols;
            grid[x][y] = 1;
        }
    }*/

    // Initialize the grid with random alive cells
    void initializeGrid(int aliveCells) {
        srand(time(0)); // Seed for random number generation
        int placedCells = 0;
        while (placedCells < aliveCells) {
            int x = rand() % rows;
            int y = rand() % cols;

            // Check if the cell is already alive
            if (grid[x][y] == 0) {
                grid[x][y] = 1; // Mark the cell as alive
                placedCells++;
            }
        }
        initialGrid = grid;
    }

    // Overloading the << operator to print the grid
    /*friend ostream& operator<<(ostream& os, const GameOfLife& game) {
        os << "\033[H";
        //int stepCount = 1;
        //system("CLS"); // Clears the console (use "CLS" for Windows)
        //os << "Step " << game.stepCount << ": " << "\n";
        for (int i = 0; i < game.rows; ++i) {
            for (int j = 0; j < game.cols; ++j) {
                os << ". ";
                os << (game.grid[i][j] ? 'O' : ' ') << " ";
            }
            os << ".\n";
        }
        //game.stepCount++;
        return os;
    }*/

    // Run the game for a given number of steps
    void run(int steps) {
        system("CLS");
        for (int i = 0; i < steps; ++i) {
            if (i == 0) {
                initialGrid = grid;
            }
            displayGrid();

            thread blockCheckThread(&GameOfLife::checkForBlocks, this);
            blockCheckThread.join();
            thread horizontalBeehiveThread(&GameOfLife::checkForHorizontalBeehive, this);
            horizontalBeehiveThread.join();

            if (blockFound) {
                cout << "Simulation paused because a block was detected\n";
                cout << "Press 'r' to resume or 's' to save or 'q' to quit.\n";

                // Pause the simulation and wait for user input
                while (true) {
                    char ch = _getch();
                    if (ch == 'r' || ch == 'R') {
                        break; // Resume the simulation
                    }
                    else if (ch == 'q' || ch == 'Q') {
                        exit(0); // Quit the simulation
                    }
                    else if (ch == 's' || ch == 'S') {
                        string fileName;
                        char iOrC;
                        cout << "Enter filename to save to: ";
                        cin >> fileName;
                        cout << endl;
                        cout << "Press 'i' if you want to save the grid of step 1 or press 'c' if you want to save the current step. \n";
                        cin >> iOrC;
                        saveGame(fileName, iOrC);
                        cout << "\nSimulation saved. Press 'R' to resume or any other key to quit.\n";
                        char secondChoice = _getch();
                        if (secondChoice == 'r' || secondChoice == 'R') {
                            continue;
                        }
                        else {
                            return;
                        }
                    }
                }
            }

            if (horizontalBeehiveFound) {
                cout << "Simulation paused because a horizontal beehive was detected\n";
                cout << "Press 'r' to resume or 's' to save or 'q' to quit.\n";

                // Pause the simulation and wait for user input
                while (true) {
                    char ch = _getch();
                    if (ch == 'r' || ch == 'R') {
                        break; // Resume the simulation
                    }
                    else if (ch == 'q' || ch == 'Q') {
                        exit(0); // Quit the simulation
                    }
                    else if (ch == 's' || ch == 'S') {
                        string fileName;
                        char iOrC;
                        cout << "Enter filename to save to: ";
                        cin >> fileName;
                        cout << endl;
                        cout << "Press 'i' if you want to save the grid of step 1 or press 'c' if you want to save the current step. \n";
                        cin >> iOrC;
                        saveGame(fileName, iOrC);
                        cout << "\nSimulation saved. Press 'R' to resume or any other key to quit.\n";
                        char secondChoice = _getch();
                        if (secondChoice == 'r' || secondChoice == 'R') {
                            continue;
                        }
                        else {
                            return;
                        }
                    }
                }
            }


            //cout << "Step " << i + 1 << ": \n";
            //cout << *this;
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'p' || ch == 'P') {
                    cout << "\nSimulation paused. Press 'R' to resume or 'S' to save the grid. \n";
                    char choice = _getch();
                    if (choice == 'r' || choice == 'R') {
                        continue;
                    }
                    else if (choice == 's' || choice == 'S') {
                        string fileName;
                        char iOrC;
                        cout << "Enter filename to save to: ";
                        cin >> fileName;
                        cout << endl;
                        cout << "Press 'i' if you want to save the grid of step 1 or press 'c' if you want to save the current step. \n";
                        cin >> iOrC;
                        saveGame(fileName, iOrC);
                        cout << "Simulation saved. Press 'R' to resume or any other key to quit.\n";
                        char secondChoice = _getch();
                        if (secondChoice == 'r' || secondChoice == 'R') {
                            continue;
                        }
                        else {
                            return;
                        }
                        //return;
                    }
                }
            }


            updateGrid();
            Sleep(200); //200 for regular simulation, increase for shape testing
        }
    }

    void saveGame(const string& fileName, char gridToSave) {
        ofstream file(fileName);
        if (file.is_open()) {
            file << rows << " " << cols << "\n";
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (gridToSave == 'c' || gridToSave == 'C') {
                    file << grid[i][j] << " ";
                    }
                    else {
                        file << initialGrid[i][j] << " ";
                    }
                }
                file << "\n";
            }
            file.close();
            cout << "Game has been saved to " << fileName << "\n";
        }
        else {
            cout << "Unable to save game. \n";
        }
    }

    //Possible refactoring of this function could be to make it void
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
            cout << "Error: Unable to load the game.\n";
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
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter an integer value.\n";
        }
        else {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

int main() {
    int rows, cols, aliveCells, steps;
    char choice;
    GameOfLife* game = nullptr;

    cout << "Do you want to load a saved game? (Y/N): ";
    cin >> choice;

    if (choice == 'Y' || choice == 'y') {
        string fileName;
        cout << "Enter the filename to load: ";
        cin >> fileName;
        game = new GameOfLife(0, 0);
        if (game->loadGame(fileName)) {
            cout << "Enter the number of steps to run: ";
            steps = getValidatedInput<int>("Steps: ");
            game->run(steps);
        }
    } else {
            rows = getValidatedInput<int>("Enter the number of rows: ");
            cols = getValidatedInput<int>("Enter the number of columns: ");
            aliveCells = getValidatedInput<int>("Enter the number of initial alive cells: ");
            steps = getValidatedInput<int>("Enter the number of steps: ");

            game = new GameOfLife(rows, cols);
            game->initializeGrid(aliveCells);
            system("CLS");
            game->run(steps);
    }

    delete game;
    return 0;

    /*cout << "Enter the number of rows: ";
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

    return 0;*/
}
