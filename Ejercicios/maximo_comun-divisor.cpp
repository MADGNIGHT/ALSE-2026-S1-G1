#include <iostream>

int main()
{
    int num1, num2;
    std::cout << "Ingrease un numero:";
    std::cin >> num1;
    std::cout << "Ingrease otro numero:";
    std::cin >> num2;

    if (num1 < 0 || num2 < 0)
    {
        num1 = -num1;
        num2 = -num2;
    }
    while (num2 != 0)
    {
        int residuo = num1 % num2;
        num1 = num2;
        num2 = residuo;
    }
    std::cout << "El maximo comun divisor es: " << num1;
    return 0;
}