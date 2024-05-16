#include <iostream>
#include <vector>
#include <unistd.h>

//#define DEBUG

using namespace std;

// CHOOSE DESIGN OF A LIVING CELL HERE
char cell = '0';

// width of grid
const int WIDTH = 20;
// height of grid
const int HEIGHT = 20;

void initialize(vector<vector<int>>& grid);
void printGrid(const vector<vector<int>>& grid);
int countNeighbors(const vector<vector<int>>& grid, int x, int y);
void update(vector<vector<int>>& grid, bool& still_alive);


int main() {
    // create grid with WIDTH and HEIGHT
    vector<vector<int>> grid(HEIGHT, vector<int>(WIDTH, 0));
    initialize(grid);

    bool still_alive = true;

    while(still_alive) {
        printGrid(grid);

#ifdef DEBUG
        // mark a new loop
        cout << "-------------" << endl;
#endif

        update(grid, still_alive);
        // 500 milliseconds wait
        usleep(500000);
    }
    // no living cell left in the grid
    cout << "Population is dead." << endl;
    return 0;
}

void initialize(vector<vector<int>>& grid) {
    // SET STARTER CELLS HERE

    // example pattern: blinker (oscillates between horizontal and vertical)
    grid[2][2] = 1;
    grid[2][3] = 1;
    grid[2][4] = 1;

    // example pattern: tetris rectangle (repeating between plus and circle)
    grid[5][5] = 1;
    grid[5][6] = 1;
    grid[5][7] = 1;
    grid[4][6] = 1;

    // example pattern: glider (travels across screen)
    grid[5][5] = 1;
    grid[5][6] = 1;
    grid[5][7] = 1;
    grid[4][7] = 1;
    grid[3][6] = 1;

    // random pattern
    grid[8][5] = 1;
    grid[1][3] = 1;
    grid[5][7] = 1;
    grid[3][6] = 1;
    grid[7][7] = 1;
}

void printGrid(const vector<vector<int>>& grid) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // print grid: for living cells choose character at top, dead cells are empty
            cout << (grid[y][x] ? cell : ' ');
        }
        cout << endl;
    }
}

int countNeighbors(const vector<vector<int>>& grid, int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // its own position
            if (i == 0 && j == 0) continue;

            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                // position is in the grid, so neighbor value gets added to the counter
                count += grid[ny][nx];
            }
        }
    }
    return count;
}

void update(vector<vector<int>>& grid, bool& still_alive) {
    // set alive to false, if there still is a living cell it will be set to true again
    still_alive = false;
    int neighbors;

    vector<vector<int>> new_grid = grid;
    // go through each column of cells for each line
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // get number of living neighbor cells
            neighbors = countNeighbors(grid, x, y);

            if (grid[y][x] == 1) {
#ifdef DEBUG
                // check if correct cells are found
                cout << "Cell: " + to_string(y) + "," + to_string(x) + " is alive" << endl;
                // check number of neighbors
                cout << neighbors << endl;
#endif

                still_alive = true;
                // cell is alive, if it has less than 2 or more than 3 neighbors it dies
                if (neighbors < 2 || neighbors > 3) {
#ifdef DEBUG
                    cout << "Kill Cell\n" << endl;
#endif

                    new_grid[y][x] = 0;
                }
            } else {
                // cell is dead
                if (neighbors == 3) {
                    // there are exactly 3 neighbors, so cell becomes alive
#ifdef DEBUG
                    cout << "Cell: " + to_string(y) + "," + to_string(x) + " is born\n" << endl;
#endif

                    new_grid[y][x] = 1;
                }
            }
        }
    }
    // change grid to new evolution
    grid = new_grid;
}
