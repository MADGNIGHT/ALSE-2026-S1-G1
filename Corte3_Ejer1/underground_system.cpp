/**
 * @file underground_system.cpp
 * @brief Sistema de transporte con tiempos promedio.
 */

#include <unordered_map>
#include <string>
using namespace std;

/**
 * @class UndergroundSystem
 * @brief Calcula tiempos promedio entre estaciones.
 */
class UndergroundSystem
{
private:
    unordered_map<int, pair<string, int>> checkMap;
    unordered_map<string, pair<int, int>> travelD;

public:
    UndergroundSystem() {}

    /** @brief Registro de entrada */
    void checkIn(int id, string stationName, int t)
    {
        checkMap[id] = {stationName, t};
    }

    /** @brief Registro de salida */
    void checkOut(int id, string stationName, int t)
    {
        auto [startStation, startTime] = checkMap[id];
        checkMap.erase(id);

        string key = startStation + "-" + stationName;
        int duration = t - startTime;

        travelD[key].first += duration;
        travelD[key].second += 1;
    }

    /** @brief Tiempo promedio */
    double getAverageTime(string startStation, string endStation)
    {
        string key = startStation + "-" + endStation;
        auto [total, count] = travelD[key];
        return (double)total / count;
    }
};
