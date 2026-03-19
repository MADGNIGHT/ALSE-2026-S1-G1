#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Spreadsheet
{
public:
    vector<vector<int>> grid; // matriz que representa el spreadsheet por filas y columnas

    Spreadsheet(int rows)
    {
        grid = vector<vector<int>>(rows, vector<int>(26, 0)); // Generar un spreadsheet con el numero de filas dado y 26 columnas (A-Z) inicializadas en 0
    }

    void setCell(string cell, int value) // El que recibe la info de una celda y un valor para asignar a esa celda
    {
        int col = cell[0] - 'A';
        int row = 0;

        for (int i = 1; i < cell.size(); i++) // Convertir la parte numérica de la celda a un índice de fila
        {
            row = row * 10 + (cell[i] - '0');
        }

        row--;
        grid[row][col] = value;
    }

    void resetCell(string cell) // Cambiar el valor de la celda dada a 0
    {
        int col = cell[0] - 'A';
        int row = 0;

        for (int i = 1; i < cell.size(); i++)
        {
            row = row * 10 + (cell[i] - '0');
        }

        row--;
        grid[row][col] = 0;
    }

    int getValue(string formula) // Recibir una formula, evaluar su resultado y devolverlo
    {
        formula = formula.substr(1); // Eliminar el signo '=' al inicio de la fórmula

        int pos = formula.find('+'); // Encontrar la posición del operador '+' en la fórmula

        string left = formula.substr(0, pos);   // Obtener la parte izquierda de la fórmula
        string right = formula.substr(pos + 1); // Obtener la parte derecha de la fórmula

        int val1 = 0, val2 = 0;

        // LEFT SIDE
        if (left[0] >= '0' && left[0] <= '9') // Si la parte izquierda es un número, convertirlo a entero
        {
            for (char c : left) // For para convertir la parte numérica de la fórmula a un valor entero
            {
                val1 = val1 * 10 + (c - '0');
            }
        }
        else // Si la parte izquierda es una referencia a una celda, obtener su valor del grid
        {
            int col = left[0] - 'A';
            int row = 0;

            for (int i = 1; i < left.size(); i++) // For para convertir la parte numérica de la referencia a celda a un índice de fila
            {
                row = row * 10 + (left[i] - '0');
            }

            row--;
            val1 = grid[row][col];
        }

        // RIGHT SIDE
        if (right[0] >= '0' && right[0] <= '9') // Si la parte derecha es un número, convertirlo a entero
        {
            for (char c : right) // For para convertir la parte numérica de la fórmula a un valor entero
            {
                val2 = val2 * 10 + (c - '0');
            }
        }
        else // Si la parte derecha es una referencia a una celda, obtener su valor del grid
        {
            int col = right[0] - 'A';
            int row = 0;

            for (int i = 1; i < right.size(); i++) // For para convertir la parte numérica de la referencia a celda a un índice de fila
            {
                row = row * 10 + (right[i] - '0');
            }

            row--;
            val2 = grid[row][col];
        }

        return val1 + val2;
    }
};
