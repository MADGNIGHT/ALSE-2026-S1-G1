/**
 * @file meetup.cpp
 * @brief Calcula días específicos en un calendario.
 */

#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * @brief Calcula el día de la semana usando Zeller.
 */
int dayOfWeek(int y, int m, int d)
{
    if (m < 3)
    {
        m += 12;
        y--;
    }
    int time = y % 100;
    int time_divide = y / 100;
    int count = (d + 13 * (m + 1) / 5 + time + time / 4 + time_divide / 4 + 5 * time_divide) % 7;
    return (count + 6) % 7;
}

/**
 * @class Meetup
 * @brief Determina fechas según reglas (first, second, etc).
 */
class Meetup
{
public:
    /**
     * @brief Obtiene el día según el horario.
     */
    static int getDay(int year, int month, string weekday, string schedule)
    {
        map<string, int> days = {
            {"Sunday", 0}, {"Monday", 1}, {"Tuesday", 2}, {"Wednesday", 3}, {"Thursday", 4}, {"Friday", 5}, {"Saturday", 6}};

        int target = days[weekday];
        vector<int> matches;

        for (int d = 1; d <= 31; d++)
        {
            if (dayOfWeek(year, month, d) == target)
                matches.push_back(d);
        }

        if (schedule == "first")
            return matches[0];
        if (schedule == "second")
            return matches[1];
        if (schedule == "third")
            return matches[2];
        if (schedule == "fourth")
            return matches[3];
        if (schedule == "last")
            return matches.back();

        for (int d : matches)
            if (d >= 13 && d <= 19)
                return d;

        return -1;
    }
};