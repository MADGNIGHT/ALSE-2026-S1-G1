class ParkingSystem
{
public:
    int big;
    int medium;
    int small;

    ParkingSystem(int b, int m, int s)
    {
        big = b;
        medium = m;
        small = s;
    }

    bool addCar(int carType)
    {

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