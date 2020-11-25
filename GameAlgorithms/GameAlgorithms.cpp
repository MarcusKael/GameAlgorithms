// GameAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

void printArray(vector<int> arr, size_t n) {
    // Esto imprime un array entregado
    size_t i;
    cout << " ";
    for (i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void remove(vector<int> &v)
{
    // Esto elimina duplicados
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

vector<int> insertionSort(vector<int> arr) {
    size_t i, j;
    int key;
    for (i = 1; i < arr.size(); i++) {
        key = arr[i];//take value
        j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;   //insert in right place
    }
    return arr;
}

int chebyshevDistance(int** maze, int x1, int y1, int x2, int y2) {
    // Esto calcula la distancia chebyshev
    // cout << "\n Chebycheck: " << maze[x1][y1] << " ";
    if (maze[x1][y1] > 1) {
        return 100000;
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    return (dx + dy) - min(dx, dy);
}

void solLen(int** sol, size_t size) {
    // Esto imprime por pantalla el tamaño de la solúción
    int counter = 0;
    size_t i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (sol[i][j] == 1) {
                counter++;
            }
        }
    }

    cout << "\n Size of solution: " << counter;
}

void printSolution(int** sol, size_t size) {
    // Esto imprime por pantaña la solución
    size_t i, j;
    for (i = 0; i < size; i++) {
        cout << " ";
        for (j = 0; j < size; j++) {
            cout << sol[i][j];
            cout << " ";
        }
        cout << "\n";
    }

    solLen(sol, size);
}

bool isSafe(int** maze, int x, int y, int g_x, int g_y, int** sol, vector<int> distances, int iteration, size_t size) {
    // cout << "\n Testing coordinates (isSafe): (" << x << ", " << y << ")" << endl;

    // Si la posición está dentro de los límites del laberinto, si el mismo espacio en solución no es 1, si la distancia chebyshev entre el punto y la meta es menor o igual a la menor
    // distancia en el arreglo de distancias y el espacio en el laberinto no es un 2, entonces el espacio es válido.
    if (x >= 0 && x < size && y >= 0 && y < size && maze[x][y] >= 0 && sol[x][y] != 1 && chebyshevDistance(maze, x, y, g_x, g_y) <= distances[iteration] && maze[x][y] != 2) {
        // cout << "\n Valid coordinate \n";
        return true;
    }
    // cout << "\n Not valid coordinate \n";
    return false;
}

bool solveMazeUtil(int** maze, int x, int y, int g_x, int g_y, int** sol, int limit, vector<int> distances, bool diagonal, int iteration, size_t size) {

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

    // Detección si el espacio actual es seguro
    if (isSafe(maze, x, y, g_x, g_y, sol, distances, 0, size) == true) {
        sol[x][y] = 1;

        // cout << "\n Distance from actual point to goal: " << chebyshevDistance(maze, x, y, g_x, g_y) << endl;

        // cout << "\n Actual solution: " << endl;
        // printSolution(sol, size);

        for (i = 1; i < size; i++) {

            // Actualizar camino caminable
            if (maze[x][y] == 0) {
                maze[x][y] = 1;
            }

            // cout << "\n\n Calculating distances..." << endl;

            // Calcular distancias adyacentes
            if (x + i < size && x + i >= 0)
                distances.push_back(chebyshevDistance(maze, x + i, y, g_x, g_y));
            if (y + i < size && y + i >= 0)
                distances.push_back(chebyshevDistance(maze, x, y + i, g_x, g_y));
            if (x - i < size && x - i >= 0)
                distances.push_back(chebyshevDistance(maze, x - i, y, g_x, g_y));
            if (y - i < size && y - i >= 0)
                distances.push_back(chebyshevDistance(maze, x, y - i, g_x, g_y));

            // Se insertan las distancias en un arreglo, se ordenan y eliminan duplicados
            distances_the_revenge = insertionSort(distances);
            remove(distances_the_revenge);

            /*
            cout << "\n Distances array after insertionSort: " << endl;
            printArray(distances_the_revenge, distances_the_revenge.size());
            cout << "\n Distances size: " << distances.size() << endl;
            cout << "\n i value: " << i << endl;
            cout << "\n Best distance: " << distances_the_revenge[0] << endl;
            */

            // Recursiones
            if (i <= maze[x][y]) {
                if (solveMazeUtil(maze, x + i, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration, size) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x, y + i, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration, size) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x - i, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration, size) == true) {
                    return true;
                }
                if (solveMazeUtil(maze, x, y - i, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration, size) == true) {
                    return true;
                }
            }
            else if (i == 2) {

                // Backtracking algorithm
                // cout << "\n This is banning direction: " << x << ", " << y << endl;
                // distances_the_revenge[0] = 100000;
                maze[x][y] = 2;
                sol[x][y] = 9;
                solveMazeUtil(maze, x, y, g_x, g_y, sol, limit - 1, distances_the_revenge, diagonal, iteration, size);
            }
        }

        sol[x][y] = 0;
        return false;
    }
    return false;
}

bool solveMaze(int** maze, int x, int y, int g_x, int g_y, int limit, bool diagonal, size_t size) {
    
    // Lista de distancias
    vector<int> d_list;

    // Se agrega valor grande para comparación
    d_list.push_back(100000);

    int iterCheck = 0;

    cout << "\n Trying to solve maze: " << endl;
    printSolution(maze, size);
    cout << "\n Start in: (" << x << ", " << y << ")";
    cout << "\n End in: (" << g_x << ", " << g_y << ")" << endl;
    cout << "\n 0 = Walkable space\n 1 = Selected solution\n 2 = Impassable space" << endl;

    int i, j;

    // Crear arreglo 2D solución
    int** sol = new int* [size];
    for (size_t k = 0; k < size; k++) {
        sol[k] = new int[size];
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (maze[i][j] > 1) {
                sol[i][j] = maze[i][j];
                continue;
            }
            sol[i][j] = 0;
        }
    }

    // Inicio recursión
    if (solveMazeUtil(maze, x, y, g_x, g_y, sol, limit, d_list, diagonal, iterCheck, size) == false) {
        cout << "\n Solution doesn't exist" << endl;
        printSolution(sol, size);
        return false;
    }

    cout << "\n Found solution: " << endl;
    printSolution(sol, size);
    return true;
}

size_t fileSize(string fileName) {
    // Esto determina el tamaño del laberinto en el archivo entregado
    ifstream mazeFile;
    string line;
    // string fileName = "mazeFile.txt";
    mazeFile.open(fileName, ios::in);

    // Checking error
    if (mazeFile.fail()) {
        cout << "Error opening file" << endl;
        exit(0);
    }

    while (!mazeFile.eof()) {
        getline(mazeFile, line);
    }
    mazeFile.close();

    size_t liSi = line.size() - (line.size() / 2);

    return liSi;
}

void readFile(string fileName, int** matrix) {
    // Esto lee el laberinto en el archivo entregado y lo carga en una matriz
    ifstream mazeFile;
    mazeFile.open(fileName, ios::in);

    // Checking error
    if (mazeFile.fail()) {
        cout << "Error opening file" << endl;
        exit(0);
    }

    size_t SIZE = fileSize(fileName), i = 0, j = 0;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            mazeFile >> matrix[i][j];
        }
    }
    mazeFile.close();
}

int main(){

    /* MAIN */

    size_t size;

    size = fileSize("mazeFile.txt");

    // Building maze
    int** maze = new int* [size]; // each element is a pointer to an array.
    for (size_t i = 0; i < size; ++i) {
        maze[i] = new int[size];
    }

    cout << "\n Maze size: " << size << endl;

    readFile("mazeFile.txt", maze);

    solveMaze(maze, 9, 2, 0, 9, 20, false, size);
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