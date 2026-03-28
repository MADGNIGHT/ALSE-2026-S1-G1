/**
 * @file authentication.cpp
 * @brief Implementación de un gestor de autenticación basado en tokens con tiempo de expiración.
 */
#include <iostream>
#include <string>
using namespace std;
/**
 * @class AuthenticationManager
 * @brief Gestiona tokens con tiempo de vida limitado (TTL).
 */

class AuthenticationManager
{
public:
    int timeToLive;
    unordered_map<string, int> tokens; // tokenId -> expirationTime
                                       /**
                                        * @brief Constructor del gestor.
                                        * @param timeToLive Tiempo de vida de los tokens.
                                        */
    AuthenticationManager(int timeToLive)
    {
        this->timeToLive = timeToLive;
    }
    /**
     * @brief Genera un nuevo token.
     * @param tokenId Identificador del token.
     * @param currentTime Tiempo actual.
     */
    void generate(string tokenId, int currentTime)
    {
        tokens[tokenId] = currentTime + timeToLive;
    }

    /**
     * @brief Renueva un token si no ha expirado.
     * @param tokenId Identificador del token.
     * @param currentTime Tiempo actual.
     */
    void renew(string tokenId, int currentTime)
    {
        if (tokens.count(tokenId) && tokens[tokenId] > currentTime)
        {
            tokens[tokenId] = currentTime + timeToLive;
        }
    }
    /**
     * @brief Cuenta los tokens no expirados.
     * @param currentTime Tiempo actual.
     * @return Número de tokens válidos.
     */

    int countUnexpiredTokens(int currentTime)
    {
        int count = 0;

        for (auto &p : tokens)
        {
            if (p.second > currentTime)
            {
                count++;
            }
        }

        return count;
    }
};