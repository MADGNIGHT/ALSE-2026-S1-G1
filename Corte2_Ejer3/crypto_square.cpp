/**
 * @file crypto_square.cpp
 * @brief Implementa el cifrado tipo "crypto square".
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;
/**
 * @brief Elimina espacios de una frase.
 * @param frase Texto original.
 * @return Texto sin espacios.
 */
string remove_Spacee(string frase)
{
    frase.erase(remove(frase.begin(), frase.end(), ' '), frase.end());
    return frase;
}

/**
 * @brief Divide la frase en filas y columnas.
 * @param c Columnas.
 * @param r Filas.
 * @param frase Texto.
 * @return Vector de strings organizado.
 */
vector<string> frase_lista(int c, int r, string frase)
{
    vector<string> lista;
    string text = "";
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            char letra = frase[j];
            text += letra;
        }
        frase.erase(0, 2);
        lista.push_back(text);
        text = "";
    }
    return lista;
}
/**
 * @brief Calcula dimensiones adecuadas para el cifrado.
 * @param frase Texto.
 * @return Matriz representada como vector.
 */
vector<string> tamaño(string frase)
{
    vector<string> lista;
    int n = frase.size();
    int r = floor(sqrt(n));
    int c = r;
    string text = "";
    if (r * c >= n)
    {
        lista = frase_lista(c, r, frase);
    }
    else if (r * (c + 1) >= n)
    {
        c++;
        lista = frase_lista(c, r, frase);
    }
    else
    {
        r++;
        c++;
        lista = frase_lista(c, r, frase);
    }
    return lista;
}
/**
 * @brief Genera el cifrado reorganizando columnas.
 * @param lista Matriz de texto.
 * @return Texto cifrado en partes.
 */
vector<string> cryptoing(vector<string> lista)
{
    vector<string> crypto;
    for (int i = 0; i < lista.size(); i++)
    {
        string text = "";
        int c = lista[i].size();
        for (int j = 0; j < c; j++)
        {
            string frase = lista[j];
            char letra = frase[i];
            text += letra;
        }
        crypto.push_back(text);
    }
    return crypto;
}
/**
 * @brief Construye el mensaje final cifrado.
 * @param crypto Vector cifrado.
 * @return String final.
 */
string message(vector<string> crypto)
{
    string show;
    for (int i = 0; i < crypto.size(); i++)
    {
        show += crypto[i] + " ";
    }
    return show;
}
/**
 * @brief Main.
 */

int main()
{
    string frase;
    cout << "Ingrese la frase a cifrar: ";
    getline(cin, frase);
    string frase_sin_espacios = remove_Spacee(frase);
    vector<string> lista = tamaño(frase_sin_espacios);
    vector<string> crypto = cryptoing(lista);
    string show = message(crypto);
    cout << "Frase cifrada: " << show << endl;
}
