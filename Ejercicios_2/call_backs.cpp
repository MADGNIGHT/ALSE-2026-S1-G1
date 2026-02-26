#include <iostream>
#include <functional>
#include <string>

double add(double A, double B)
{
    return A + B;
}

double subtract(double A, double B)
{
    return A - B;
}

double multiply(double A, double B)
{
    return A * B;
}

double divide(double A, double B)
{
    if (B == 0)
    {
        throw std::invalid_argument("Division by zero is not allowed.");
    }
    return A / B;
}

int main(int arg, char *argv[])
{
    if (arg != 4)
    {
        std::cout << "Has ingresado valores afuera del rango aceptado.";
        std::cout << "Has usado " << argv[0] << " cuando deberia ser <num1> <operador> <num2>";
        return 1;
    }
    double num1 = std::stod(argv[1]);
    double num2 = std::stod(argv[3]);
    std::string operador = argv[2];

    std::function<double(double, double)> operation;

    if (operador == "+")
    {
        operation = add;
    }
    else if (operador == "-")
    {
        operation = subtract;
    }
    else if (operador == "*")
    {
        operation = multiply;
    }
    else if (operador == "/")
    {
        operation = divide;
    }
    else
    {
        std::cout << "El operador usado no es valido!";
        std::cout << "Has usado " << operador << " cuando deberia ser +, -, *, o /";
        return 1;
    }
    double result = operation(num1, num2);
    std::cout << "El resultado de es " << result << std::endl;
    return 0;
}