#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int num, Origennum, remainder, cal = 0, tamaño = 0, counter = 0;
    bool Flag = true;
    cout << "Ingrese un numero: ";
    cin >> num;
    Origennum = num;

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
    while (counter < tamaño)
    {
        cout << "4:" << num << endl;
        remainder = num % 10;
        num = num / 10;
        cout << "5:" << remainder << endl;
        cout << "6+:" << cal << endl;
        cal = cal + pow(remainder, tamaño);
        cout << "6:" << cal << endl;
        counter++;
    }
    cout << "7:" << cal << endl;
    cout << "8:" << Origennum << endl;
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