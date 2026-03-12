#include <iostream>
using namespace std;

const double EARTH_YEAR_SECONDS = 31557600.0;

double ageOnMercury(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 0.2408467;
}

double ageOnVenus(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 0.61519726;
}

double ageOnEarth(long seconds)
{
    return seconds / EARTH_YEAR_SECONDS;
}

double ageOnMars(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 1.8808158;
}

double ageOnJupiter(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 11.862615;
}

double ageOnSaturn(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 29.447498;
}

double ageOnUranus(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 84.016846;
}

double ageOnNeptune(long seconds)
{
    return (seconds / EARTH_YEAR_SECONDS) / 164.79132;
}

int main()
{

    long seconds;

    cout << "Enter age in seconds: ";
    cin >> seconds;

    cout << "Mercury years: " << ageOnMercury(seconds) << endl;
    cout << "Venus years: " << ageOnVenus(seconds) << endl;
    cout << "Earth years: " << ageOnEarth(seconds) << endl;
    cout << "Mars years: " << ageOnMars(seconds) << endl;
    cout << "Jupiter years: " << ageOnJupiter(seconds) << endl;
    cout << "Saturn years: " << ageOnSaturn(seconds) << endl;
    cout << "Uranus years: " << ageOnUranus(seconds) << endl;
    cout << "Neptune years: " << ageOnNeptune(seconds) << endl;

    return 0;
}