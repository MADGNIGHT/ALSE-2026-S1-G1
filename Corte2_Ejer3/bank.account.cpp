/**
 * @file bank.account.cpp
 * @brief Implementación de una cuenta bancaria segura para hilos.
 */
#include <iostream>
#include <mutex>
using namespace std;
/**
 * @class BankAccount
 * @brief Representa una cuenta bancaria con operaciones seguras en entornos concurrentes.
 */
class BankAccount
{
private:
    long long balance;
    bool isClosed;
    mutex block;

public:
    /**
     * @brief Constructor de la cuenta.
     */
    BankAccount()
    {
        balance = 0;
        isClosed = false;
    }

    /**
     * @brief Deposita dinero en la cuenta.
     * @param amount Cantidad a depositar.
     * @return true si la operación fue exitosa.
     */
    bool deposit(long long amount)
    {
        lock_guard<mutex> lock(block);

        if (isClosed)
            return false;
        balance += amount;
        return true;
    }
    /**
     * @brief Retira dinero de la cuenta.
     * @param amount Cantidad a retirar.
     * @return true si la operación fue exitosa.
     */
    bool withdraw(long long amount)
    {
        lock_guard<mutex> lock(block);

        if (isClosed || balance < amount)
            return false;
        balance -= amount;
        return true;
    }

    /**
     * @brief Obtiene el saldo actual.
     * @param result Referencia donde se guarda el saldo.
     * @return true si la cuenta está activa.
     */
    bool getBalance(long long &result)
    {
        lock_guard<mutex> lock(block);

        if (isClosed)
            return false;
        result = balance;
        return true;
    }

    /**
     * @brief Cierra la cuenta.
     * @return true si se cerró correctamente.
     */

    bool close()
    {
        lock_guard<mutex> lock(block);

        if (isClosed)
            return false;
        isClosed = true;
        return true;
    }
};