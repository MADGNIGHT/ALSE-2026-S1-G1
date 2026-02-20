#include <iostream>

int main()
{
    double P, r, A;
    int n, t;

    std::cout << "Ingrese el numero Principal: ";
    std::cin >> P;

    std::cout << "Ingrese la tasa de interes anual en forma decimal(Ejemplo: 0.05 == 5%): ";
    std::cin >> r;

    std::cout << "Ingrese el numero de veces que se aplica el interes por año:";
    std::cin >> n;

    std::cout << "Ingrese el numero de años: ";
    std::cin >> t;

    A = P * pow((1 + r / n), n * t);
    std::cout << "EL monto fianl es: " << A;
    return 0;
}