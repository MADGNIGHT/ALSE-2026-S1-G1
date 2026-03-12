#include <iostream>
#include <string>
using namespace std;

int hammingDistance(string dna1, string dna2)
{

    if (dna1.length() != dna2.length())
    {
        return -1; // error si tienen diferente tamaño
    }

    int distance = 0;

    for (int i = 0; i < dna1.length(); i++)
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

    cout << "Enter first DNA strand: ";
    cin >> dna1;

    cout << "Enter second DNA strand: ";
    cin >> dna2;

    int result = hammingDistance(dna1, dna2);

    if (result == -1)
    {
        cout << "Error: DNA strands must be of equal length." << endl;
    }
    else
    {
        cout << "Hamming Distance: " << result << endl;
    }

    return 0;
}