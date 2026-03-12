class ParkingSystem
{
public:
    int big;
    int medium;
    int small;
    // Tamaño de cada espacio de estacionamiento
    ParkingSystem(int b, int m, int s)
    {
        big = b;
        medium = m;
        small = s;
    }
    // Funcion para agregar un carro al estacionamiento
    bool addCar(int carType)
    {
        // Verificar el tipo de carro y si hay espacio disponible para ese tipo
        if (carType == 1 && big > 0)
        {
            big--;
            return true;
        }

        if (carType == 2 && medium > 0)
        {
            medium--;
            return true;
        }

        if (carType == 3 && small > 0)
        {
            small--;
            return true;
        }

        return false;
    }
};