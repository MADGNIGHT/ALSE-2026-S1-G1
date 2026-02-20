#include <iostream>

int main()
{
    int num;
    std::cout << "Ingrese un numero entero positivo: ";
    std::cin >> num;
    bool flag = true;
    bool flag2 = true;
    int counter = 0;
    while (flag == true)
    {
        if (num < 0)
        {
            num = -num;
            flag = false;
        }
        else if (num == 0)
        {
            std::cout << "Has ingresado 0, escoge otro nombre: ";
            std::cin >> num;
        }
        else
        {
            flag = false;
        }
    }
    while (flag2 == true)
    {
        if (num == 1)
        {
            std::cout << "Tu numero ya es 1, se hicieron " << counter << " interacciones para llegar a él. Proceso terminado";
            flag2 = false;
        }
        else if (num % 2 == 0)
        {
            num = num / 2;
            counter++;
        }
        else
        {
            num = (num * 3) + 1;
            counter++;
        }
    }
    return 0;
}