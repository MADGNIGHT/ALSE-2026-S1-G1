/**
 * @file clock.cpp
 * @brief Implementación de un reloj simple.
 */

#include <iostream>
#include <ctime>
using namespace std;

/**
 * @class Clock
 * @brief Representa un reloj que muestra la hora actual del sistema.
 */
class Clock
{
public:
    /**
     * @brief Muestra la hora actual en formato HH:MM:SS.
     */
    static void showCurrentTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        cout << "Hora actual: ";
        cout << ltm->tm_hour << ":"
             << ltm->tm_min << ":"
             << ltm->tm_sec << endl;
    }
};

/**
 * @brief Función principal para ejecutar el reloj.
 */
int main()
{
    Clock::showCurrentTime();
    return 0;
}