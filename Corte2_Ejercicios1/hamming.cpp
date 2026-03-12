#include <iostream>
#include <string>
using namespace std;

int hammingDistance(string dna1, string dna2) // convertir la estructura de adn a string para comparar cada caracter y contar las diferencias
{

    if (dna1.length() != dna2.length()) // Verificar que las cadenas de ADN tengan la misma longitud
    {
        return -1; // error si tienen diferente tamaño
    }

    int distance = 0;

    for (int i = 0; i < dna1.length(); i++) // Verificar cada caracter de las cadenas de ADN y contar las diferencias
    {
        if (dna1[i] != dna2[i])
        {
            distance++;
        }
    }

    return distance;
}

int main()
{

    string dna1, dna2;

    cout << "Enter first DNA strand: "; // Primera cadena de ADN
    cin >> dna1;

    cout << "Enter second DNA strand: "; // Segunda cadena de ADN
    cin >> dna2;

    int result = hammingDistance(dna1, dna2); // Llamado de función para calcular la distancia de Hamming

    if (result == -1) // resultado
    {
        cout << "Error: DNA strands must be of equal length." << endl;
    }
    else
    {
        cout << "Hamming Distance: " << result << endl;
    }

    return 0;
}