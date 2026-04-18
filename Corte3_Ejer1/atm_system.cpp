/**
 * @file atm_system.cpp
 * @brief Implementación de un sistema ATM simple.
 */

#include <vector>
using namespace std;

/**
 * @class ATM
 * @brief Simula un cajero automático con diferentes denominaciones.
 */
class ATM
{
private:
    vector<long long> notes;                      ///< Cantidad de billetes disponibles
    vector<int> values = {20, 50, 100, 200, 500}; ///< Valores de los billetes

public:
    /**
     * @brief Constructor que inicializa el ATM sin dinero.
     */
    ATM()
    {
        notes = vector<long long>(5, 0);
    }

    /**
     * @brief Deposita billetes en el ATM.
     * @param banknotesCount Cantidad de cada tipo de billete.
     */
    void deposit(vector<int> bankCount)
    {
        for (int i = 0; i < 5; i++)
            notes[i] += bankCount[i];
    }

    /**
     * @brief Retira dinero del ATM.
     * @param amount Cantidad a retirar.
     * @return Vector con billetes usados o {-1} si no es posible.
     */
    vector<int> withdraw(int amount)
    {
        vector<int> used(5, 0);

        for (int i = 4; i >= 0; i--)
        {
            long long take = min((long long)(amount / values[i]), notes[i]);
            used[i] = take;
            amount -= take * values[i];
        }

        if (amount != 0)
            return {-1};

        for (int i = 0; i < 5; i++)
            notes[i] -= used[i];

        return used;
    }
};