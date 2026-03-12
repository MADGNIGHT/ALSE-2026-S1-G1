#include <iostream>
#include <cmath>
using namespace std;
// funcion principal
int main()
{ // Variables
    int num, Origennum, remainder, cal = 0, tamaño = 0, counter = 0;
    bool Flag = true;
    // conseguir numero del usuario
    cout << "Ingrese un numero: ";
    cin >> num;
    Origennum = num;
    // Verificar que el numero sea positivo y no cero
    while (Flag)
    {
        // cout << "1:" << tamaño << endl;
        num = num / 10;
        tamaño++;
        // cout << "2:" << tamaño << endl;
        if (num == 0)
        {
            Flag = false;
        }
    }
    // cout << tamaño << endl;
    num = Origennum;
    // Calcular el numero de Armstrong
    while (counter < tamaño)
    {
        cout << num << endl;
        remainder = num % 10;
        num = num / 10;
        cout << remainder << endl;
        cout << cal << endl;
        cal = cal + pow(remainder, tamaño);
        cout << cal << endl;
        counter++;
    }

    // Verificar si el numero es de Armstrong o no
    if (cal == Origennum)
    {
        cout << "El numero ingresado " << Origennum << " es un numero de Armstrong." << endl;
    }
    else
    {
        cout << " El numero ingresado " << Origennum << " no es un numero Armstroing." << endl;
    }
    return 0;
}