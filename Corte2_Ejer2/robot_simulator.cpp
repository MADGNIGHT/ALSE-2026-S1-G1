#include <iostream>
using namespace std;
// Funciones
// Funcion para cambiar la direccion hacia la derecha
string turn_right(string direction)
{
    if (direction == "N")
    {
        return "E";
    }
    else if (direction == "E")
    {
        return "S";
    }
    else if (direction == "S")
    {
        return "O";
    }
    else
    {
        return "N";
    }
}
// Funcion para cambiar la direccion hacia la izquierda
string turn_left(string direction)
{

    if (direction == "N")
    {
        return "O";
    }
    else if (direction == "E")
    {
        return "N";
    }
    else if (direction == "S")
    {
        return "E";
    }
    else
    {
        return "S";
    }
}
// Funcion para avanzar en la direccion actual
int advance(string direction, int x, int y)
{
    if (direction == "N" || direction == "S") // if para avanzar en el eje y
    {
        if (direction == "N")
        {
            y++;
        }
        else
        {
            y--;
        }
    }
    else if (direction == "E" || direction == "O") // if para avanzar en el eje x
    {
        if (direction == "E")
        {
            x++;
        }
        else
        {
            x--;
        }
    }
    return x, y;
}

int main()
{
    int x = 0, y = 0;
    cout << "Ingrese la posición inicial del robot (x y): "; // Solicitar la posición inicial del robot
    cin >> x >> y;
    cout << "Ingrese la dirección inicial del robot (N, E, S, O): "; // Solicitar la dirección inicial del robot
    string direction;
    cin >> direction;
    string orden = "";
    cout << "Ingrese la orden para el robot (R para girar a la derecha, L para girar a la izquierda, A para avanzar): "; // Argumentos para controlar el robot
    cin >> orden;
    for (char c : orden) // Evaluar cada caracter para saber que moviemnto va a realizar el robot
    {
        if (c == 'R')
        {
            direction = turn_right(direction);
        }
        else if (c == 'L')
        {
            direction = turn_left(direction);
        }
        else if (c == 'A')
        {
            int new_x, new_y = advance(direction, x, y);
            x = new_x;
            y = new_y;
        }
    }
    cout << "La posición final del robot es: (" << x << ", " << y << ")" << endl; // presentacion de las accione que tomo el robot.
    cout << "La dirección final del robot es: " << direction << endl;
    return 0;
}