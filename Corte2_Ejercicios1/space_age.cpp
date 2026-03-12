#include <iostream>
using namespace std;
// constante de tiempo
const double EYS = 31557600.0;
// funciones para calcular la edad en cada planeta
double ageOnMercury(long seconds)
{
    return (seconds / EYS) / 0.2408467;
}

double ageOnVenus(long seconds)
{
    return (seconds / EYS) / 0.61519726;
}

double ageOnEarth(long seconds)
{
    return seconds / EYS;
}

double ageOnMars(long seconds)
{
    return (seconds / EYS) / 1.8808158;
}

double ageOnJupiter(long seconds)
{
    return (seconds / EYS) / 11.862615;
}

double ageOnSaturn(long seconds)
{
    return (seconds / EYS) / 29.447498;
}

double ageOnUranus(long seconds)
{
    return (seconds / EYS) / 84.016846;
}

double ageOnNeptune(long seconds)
{
    return (seconds / EYS) / 164.79132;
}

int main()
{
    //
    long seconds;
    // Conseguir edad del usuario
    cout << "Ingrese su edad en segundos: ";
    cin >> seconds;
    // Mostrar todas las edades en cada planeta
    cout << "Edad en Mercurio: " << ageOnMercury(seconds) << endl;
    cout << "Edad en Venus: " << ageOnVenus(seconds) << endl;
    cout << "Edad en la Tierra: " << ageOnEarth(seconds) << endl;
    cout << "Edad en Marte: " << ageOnMars(seconds) << endl;
    cout << "Edad en Júpiter: " << ageOnJupiter(seconds) << endl;
    cout << "Edad en Saturno: " << ageOnSaturn(seconds) << endl;
    cout << "Edad en Urano: " << ageOnUranus(seconds) << endl;
    cout << "Edad en Neptuno: " << ageOnNeptune(seconds) << endl;

    return 0;
}