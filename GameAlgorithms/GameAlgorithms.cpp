// GameAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

void printArray(vector<int> arr, int n) {
    int i;
    cout << " ";
    for (i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void remove(vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

vector<int> insertionSort(vector<int> arr) {
    size_t i;
    int key, j;
    // cout << "\n Array size: " << arr.size() << endl;
    for (i = 1; i < arr.size(); i++) {

        key = arr[i];

        j = i - 1;

        while (j >= 0 && key < arr[j]) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    // cout << "Printing array: " << endl;
    // printArray(arr, arr.size());

    return arr;
}

int N = 10;
int chebyshevDistance(int maze[10][10], int x1, int y1, int x2, int y2) {
    if (maze[x1][y1] > 1) {
        return 100000;
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx + dy) - min(dx, dy);
}

void solLen(int sol[10][10]) {
    int counter = 0, i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (sol[i][j] == 1) {
                counter++;
            }
        }
    }

    cout << "\n Size of solution: " << counter;
}

void printSolution(int sol[10][10]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        cout << " ";
        for (j = 0; j < 10; j++) {
            cout << sol[i][j];
            cout << " ";
        }
        cout << "\n";
    }

    solLen(sol);
}

bool isSafe(int maze[10][10], int x, int y, int g_x, int g_y, int sol[10][10], vector<int> distances, int iteration) {
    cout << "\n Testing coordinates (isSafe): (" << x << ", " << y << ")" << endl;

    if (x >= 0 && x < 10 && y >= 0 && y < 10 && maze[x][y] >= 0 && sol[x][y] != 1 && chebyshevDistance(maze, x, y, g_x, g_y) <= distances[iteration] && maze[x][y] != 2) {
        cout << "\n Coordenada valida \n";
        return true;
    }
    // cout << "\n Coordenada no es valida \n";
    return false;
}

bool solveMazeUtil(int maze[10][10], int x, int y, int g_x, int g_y, int sol[10][10], int limit, vector<int> distances, bool diagonal, int iteration) {

    int i;
    vector<int> distances_the_revenge;

    // Límite de movimientos no implementado
    /* if (limit <= 0) {
        return true;
    }*/

    // Si la posición es igual a la meta: true
    if (x == g_x && y == g_y && maze[x][y] >= 1) {
        sol[x][y] = 1;
        return true;
    }

    if (isSafe(maze, x, y, g_x, g_y, sol, distances, 0) == true) {
        sol[x][y] = 1;

        // cout << "\n Distance from actual point to goal: " << chebyshevDistance(maze, x, y, g_x, g_y) << endl;

        cout << "\n Actual solution: " << endl;
        printSolution(sol);

        for (i = 1; i < 10; i++) {

            if (maze[x][y] == 0) {
                maze[x][y] = 1;
            }

            distances.push_back(chebyshevDistance(maze, x + i, y, g_x, g_y));
            distances.push_back(chebyshevDistance(maze, x, y + i, g_x, g_y));
            distances.push_back(chebyshevDistance(maze, x - i, y, g_x, g_y));
            distances.push_back(chebyshevDistance(maze, x, y - i, g_x, g_y));

            distances_the_revenge = insertionSort(distances);

            remove(distances_the_revenge);

            /* cout << "\n Distances array after insertionSort: " << endl;

            printArray(distances_the_revenge, distances_the_revenge.size());

            /* cout << "\n Distances size: " << distances.size() << endl;

            cout << "\n i value: " << i << endl;

            cout << "\n Best distance: " << distances_the_revenge[0] << endl; */

            if (i <= maze[x][y]) {
                if (solveMazeUtil(maze, x + i, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x, y + i, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x - i, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x, y - i, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration) == true) {
                    return true;
                }
            }
            else if (i == 2) {

                // Backtracking algorithm
                // cout << "\n This is banning direction: " << x << ", " << y << endl;
                // distances_the_revenge[0] = 100000;
                maze[x][y] = 2;
                sol[x][y] = 9;
                solveMazeUtil(maze, x, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration);
            }
        }

        sol[x][y] = 0;
        return false;
    }
    return false;
}

bool solveMaze(int maze[10][10], int x, int y, int g_x, int g_y, int limit, bool diagonal) {
    vector<int> d_list;

    d_list.push_back(100000);

    int iterCheck = 0;

    cout << "\n Trying to solve maze: " << endl;
    printSolution(maze);
    cout << "\n Start in: (" << x << ", " << y << ")";
    cout << "\n End in: (" << g_x << ", " << g_y << ")" << endl;
    cout << "\n 0 = Walkable space\n 1 = Selected solution\n 2 = Impassable space" << endl;

    int i, j, sol[10][10];

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (maze[i][j] > 1) {
                sol[i][j] = maze[i][j];
                continue;
            }
            sol[i][j] = 0;
        }
    }

    if (solveMazeUtil(maze, x, y, g_x, g_y, sol, limit, d_list, diagonal, iterCheck) == false) {
        cout << "\n Solution doesn't exist" << endl;
        printSolution(sol);
        return false;
    }

    cout << "\n Found solution: " << endl;
    printSolution(sol);
    return true;
}

int main()
{
    int maze[10][10] = {{0, 0, 0, 0, 0, 0, 2, 0, 2, 1},
                        {0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
                        {0, 2, 2, 0, 2, 2, 2, 2, 0, 0},
                        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 2, 2, 2, 2, 2, 2},
                        {0, 0, 0, 2, 2, 2, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {2, 0, 2, 2, 0, 0, 0, 0, 0, 0},
                        {0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 1, 0, 2, 0, 0, 0, 0, 0}};
    
    solveMaze(maze, 9, 2, 0, 9, 20, false);
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
