#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <conio.h>
#include <thread>
#include <windows.h>

using namespace std;

class GameOfLife {
private:
    int rows, cols;
    vector<vector<int>> grid, tempGrid, initialGrid;
    bool blockFound;
    bool horizontalBeehiveFound;
    bool verticalBeehiveFound;
    bool blinkerHToVFound;
    bool blinkerVToHFound;
    bool toadFound;
    bool toadFound2;
    bool toadFound3;
    bool toadFound4;

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
        cout << "Step " << stepCount << ": " << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols - 1; ++j) {
                cout << ". "; // Show grid boundaries
                cout << (grid[i][j] ? 'O' : ' ') << " "; // O for alive, space for dead
            }
            cout << ".\n"; // Show grid boundaries
        }
        stepCount = stepCount + 1;
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
                    }
                }
            }
        }
    }


    bool checkForHorizontalBeehive() {
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 0; j < cols - 3; ++j) {
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

    bool checkForVerticalBehhive() {
        for (int i = 0; i < rows - 3; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                if (grid[i][j] == 1 && grid[i + 1][j + 1] == 1 && grid[i + 2][j + 1] == 1
                    && grid[i + 3][j] == 1 && grid[i + 2][j - 1] == 1 && grid[i + 1][j - 1] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i + 1, j + 1) == 2)
                        && (countAliveNeighbors(i + 2, j + 1) == 2) && (countAliveNeighbors(i + 3, j) == 2)
                        && (countAliveNeighbors(i + 2, j - 1) == 2) && (countAliveNeighbors(i + 1, j - 1) == 2)) {


                        cout << endl;
                        cout << "\nTop cell of the vertical beehive is " << i << ", " << j << endl;
                        verticalBeehiveFound = true;
                        return true;
                    }

                }
            }
        }
    }

    bool checkForBlinkerHtoV() {
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                if (grid[i][j] == 1 && grid[i][j - 1] == 1 && grid[i][j + 1] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i, j - 1) == 1) && (countAliveNeighbors(i, j + 1) == 1)
                        && (countAliveNeighbors(i-1, j+1) == 2) && (countAliveNeighbors(i+1, j+1) == 2) && (countAliveNeighbors(i+1, j-1) == 2)
                        && (countAliveNeighbors(i-1, j-1) == 2)) {

                        cout << endl;
                        cout << "\nThe middle cell of a horizontal to vertical blinker is " << i << ", " << j << endl;
                        blinkerHToVFound = true;
                        return true;

                    }
                }
            }
        }
    }

    /*bool checkForBlinkerVToH() {
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                if (grid[i][j] == 1 && grid[i - 1][j] == 1 && grid[i + 1][j] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i - 1, j) == 1) && (countAliveNeighbors(i + 1, j) == 1)
                        && (countAliveNeighbors(i - 1, j + 1) == 2) && (countAliveNeighbors(i + 1, j + 1) == 2) && (countAliveNeighbors(i + 1, j - 1) == 2)
                        && (countAliveNeighbors(i - 1, j - 1) == 2)) {

                        cout << endl;
                        cout << "\nThe middle cell of a vertical to horizontal blinker is " << i << ", " << j << endl;
                        blinkerVToHFound = true;
                        return true;


                    }
                }
            }
        }
    }*/

    bool checkForToad() { // i,j is the top right of the toad
        for (int i = 1; i < rows - 2; ++i) {
            for (int j = 3; j < cols; ++j) {
                if (grid[i][j] == 1 && grid[i][j - 1] == 1 && grid[i][j - 2] == 1
                    && grid[i + 1][j - 1] == 1 && grid[i + 1][j - 2] == 1 && grid[i + 1][j - 3] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i, j - 1) == 4) && (countAliveNeighbors(i, j - 2) == 4)
                        && (countAliveNeighbors(i + 1, j - 1) == 4) && (countAliveNeighbors(i + 1, j - 2) == 4) && (countAliveNeighbors(i + 1, j - 3) == 2)
                        && (countAliveNeighbors(i + 2, j) == 1) && (countAliveNeighbors(i + 2, j - 3) == 2) && (countAliveNeighbors(i - 1, j - 3) == 1) && (countAliveNeighbors(i - 1, j) == 2)) {

                        cout << endl;
                        cout << "\nThe top right of the toad is " << i << ", " << j << endl;
                        toadFound = true;
                        return true;
                    }

                }
            }
        }
    }

    bool checkforToad2() { // i, j is the top left of the toad
        for (int i = 1; i < rows - 2; ++i) {
            for (int j = 0; j < cols - 3; ++j) {
                if (grid[i][j] == 1 && grid[i][j + 1] == 1 && grid[i][j + 2] == 1
                    && grid[i + 1][j + 1] == 1 && grid[i + 1][j + 2] == 1 && grid[i + 1][j + 3] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i, j + 1) == 4) && (countAliveNeighbors(i, j + 2) == 4)
                        && (countAliveNeighbors(i + 1, j + 1) == 4) && (countAliveNeighbors(i + 1, j + 2) == 4) && (countAliveNeighbors(i + 1, j + 3) == 2)
                        && (countAliveNeighbors(i - 1, j) == 2) && (countAliveNeighbors(i + 2, j + 3) == 2)
                        && (countAliveNeighbors(i - 1, j + 3) == 1) && (countAliveNeighbors(i + 2, j) == 1)) {

                        cout << endl;
                        cout << "\nThe top left of the toad is " << i << ", " << j << endl;
                        toadFound2 = true;
                        return true;
                    }
                }
            }
        }
    }

    bool checkForToad3() { //i, j is the top of the toad
        for (int i = 0; i < rows - 3; ++i) {
            for (int j = 1; j < cols - 2; ++j) {
                if (grid[i][j] == 1 && grid[i + 1][j] == 1 && grid[i + 2][j] == 1
                    && grid[i + 1][j + 1] == 1 && grid[i + 2][j + 1] == 1 && grid[i + 3][j + 1] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i + 1, j) == 4) && (countAliveNeighbors(i + 2, j) == 4)
                        && (countAliveNeighbors(i + 1, j + 1) == 4) && (countAliveNeighbors(i + 2, j + 1) == 4) && (countAliveNeighbors(i + 3, j + 1) == 2)
                        && (countAliveNeighbors(i, j - 1) == 2) && (countAliveNeighbors(i + 3, j + 2) == 2)
                        && (countAliveNeighbors(i + 3, j - 1) == 1) && (countAliveNeighbors(i, j + 2) == 1)) {


                        cout << endl;
                        cout << "\nThe top of the toad is " << i << ", " << j << endl;
                        toadFound3 = true;
                        return true;
                    }
                }
            }
        }
    }

    bool checkForToad4() {
        for (int i = 0; i < rows - 3; ++i) {
            for (int j = 2; j < cols - 1; ++j) {
                if (grid[i][j] == 1 && grid[i + 1][j] == 1 && grid[i + 2][j] == 1
                    && grid[i + 1][j - 1] == 1 && grid[i + 2][j - 1] == 1 && grid[i + 3][j - 1] == 1) {

                    if ((countAliveNeighbors(i, j) == 2) && (countAliveNeighbors(i + 1, j) == 4) && (countAliveNeighbors(i + 2, j) == 4)
                        && (countAliveNeighbors(i + 1, j - 1) == 4) && (countAliveNeighbors(i + 2, j - 1) == 4) && (countAliveNeighbors(i + 3, j - 1) == 2)
                        && (countAliveNeighbors(i, j + 1) == 2) && (countAliveNeighbors(i, j - 2) == 1)
                        && (countAliveNeighbors(i + 3, j - 2) == 2) && (countAliveNeighbors(i + 3, j + 1) == 1)) {


                        cout << endl;
                        cout << "\nThe top of the toad is " << i << ", " << j << endl;
                        toadFound4 = true;
                        return true;
                    }
                }
            }
        }
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


    GameOfLife operator+(const  GameOfLife& other) const {
        return *this;
    }

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

            thread verticalBeehiveThread(&GameOfLife::checkForVerticalBehhive, this);
            verticalBeehiveThread.join();

            thread blinkerHToVThread(&GameOfLife::checkForBlinkerHtoV, this);
            blinkerHToVThread.join();

            thread toadThread(&GameOfLife::checkForToad, this);
            toadThread.join();

            thread secondToadThread(&GameOfLife::checkforToad2, this);
            secondToadThread.join();

            thread thirdToadThread(&GameOfLife::checkForToad3, this);
            thirdToadThread.join();

            thread fourthToadThread(&GameOfLife::checkForToad4, this);
            fourthToadThread.join();
            
            //thread blinkerVToHThread(&GameOfLife::checkForBlinkerVToH, this);
            //blinkerVToHThread.join();

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

            if (verticalBeehiveFound) {
                cout << "Simulation paused because a vertical beehive was detected\n";
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

            if (blinkerHToVFound) {
                cout << "Simulation paused because a horizontal to vertical blinker was detected\n";
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

            /*if (blinkerVToHFound) {
                cout << "Simulation paused because a vertical to horizontal blinker was detected\n";
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
            }*/

            if (toadFound) {
                cout << "Simulation paused because a toad was detected\n";
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

            if (toadFound2) {
                cout << "Simulation paused because a toad was detected\n";
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

            if (toadFound3) {
                cout << "Simulation paused because a toad was detected\n";
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

            if (toadFound4) {
                cout << "Simulation paused because a toad was detected\n";
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
    //GameOfLife* game2 = nullptr;

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

            //game = new GameOfLife(rows, cols);
            GameOfLife game1(rows, cols);
            GameOfLife game2(rows, cols);
            game1.initializeGrid(aliveCells);
            game2.initializeGrid(aliveCells);
            GameOfLife theGame = game1 + game2;
            //game2 = new GameOfLife(rows, cols);
            //game->initializeGrid(aliveCells);
            system("CLS");
            //game->run(steps);
            theGame.run(steps);
    }

    delete game;
    //delete game2;
    return 0;
}
