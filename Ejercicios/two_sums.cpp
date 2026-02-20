#include <iostream>
#include <vector>
class Solution
{
public:
    std::vector<int> twoSum(std::vector<int> &nums, int target)
    {

        for (int flag = 0; flag < nums.size(); flag++)
        {

            for (int i = flag + 1; i < nums.size(); i++)
            {

                if (nums[flag] + nums[i] == target)
                {
                    return {flag, i};
                }
            }
        }

        return {};
    }
};