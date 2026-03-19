#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<vector<string>> board = {// Tablero de ajedrez 8x8
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"},
                                    {"-", "-", "-", "-", "-", "-", "-", "-"}};

    int x1, y1, x2, y2; // Posiciones de las reinas
    bool flag = true;   // Parametro para validar la entrada de las posiciones
    while (flag)        // Validación de las posiciones de la reina (entre 0 y 7) y conseguir valores
    {
        cout << "Ingrese la posición de la primera reina (x y) de 0 a 7: ";
        cin >> x1 >> y1;
        cout << "Ingrese la posición de la segunda reina (x y) de 0 a 7: ";
        cin >> x2 >> y2;
        if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7 || x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7)
        {
            cout << "Posición inválida. Intente de nuevo." << endl;
        }
        else
        {
            flag = false;
        }
    }

    board[x1][y1] = "Q"; // Añadir las reinas al tablero
    board[x2][y2] = "Q";
    for (int i = 0; i < 8; i++) // Mostrar el tablero con las reinas
    {
        for (int j = 0; j < 8; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    if (x1 == x2 || y1 == y2 || abs(x1 - x2) == abs(y1 - y2)) // Verificar si las reinas se logran atacar entre si y mostrar el resultado
    {
        cout << "Las reinas se atacan entre sí." << endl;
    }
    else
    {
        cout << "Las reinas no se atacan entre sí." << endl;
    }
}