/**
 * @file exam_tracker.cpp
 * @brief Sistema para registrar y consultar puntajes de exámenes en el tiempo.
 */

#include <iostream>
#include <string>
using namespace std;
/**
 * @class ExamTracker
 * @brief Permite registrar puntajes y consultar sumas en rangos de tiempo.
 */

class ExamTracker
{
public:
    vector<int> times;
    vector<int> scores_saved;
    /**
     * @brief Constructor.
     */

    ExamTracker()
    {
    }
    /**
     * @brief Registra un puntaje en un tiempo dado.
     * @param time Tiempo del registro.
     * @param score Puntaje obtenido.
     */
    void record(int time, int score)
    {
        times.push_back(time);
        if (score < 0)
        {
            scores_saved.push_back(score);
        }
        else
        {
            if (scores_saved.empty())
            {
                scores_saved.push_back(score);
            }
            else
            {
                scores_saved.push_back(scores_saved.back() + score);
            }
        }
    }
    /**
     * @brief Calcula el puntaje total en un rango de tiempo.
     * @param startTime Tiempo inicial.
     * @param endTime Tiempo final.
     * @return Suma de puntajes en el rango.
     */

    long long totalScore(int startTime, int endTime)
    {
        int lim_inferior = lower_bound(times.begin(), times.end(), startTime) - times.begin();
        int lim_superior = upper_bound(times.begin(), times.end(), endTime) - times.begin() - 1;
        if (lim_inferior > lim_superior)
        {
            return 0;
        }
        else
        {
            return scores_saved[lim_superior] - (lim_inferior > 0 ? scores_saved[lim_inferior - 1] : 0);
        }
    }
};