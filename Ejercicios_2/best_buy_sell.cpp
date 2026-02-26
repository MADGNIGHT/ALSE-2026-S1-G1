#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution
{
public:
    int maxProfit(vector<int> &prices)
    {
        int min_Price = INT_MAX;
        int max_Profit = 0;

        for (int price : prices)
        {
            if (price < min_Price)
            {
                min_Price = price;
            }
            else
            {
                int profit = price - min_Price;
                if (profit > max_Profit)
                {
                    max_Profit = profit;
                }
            }
        }
        return max_Profit;
    }
};