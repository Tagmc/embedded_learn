#include <iostream>
#include <vector>
#include <cmath>
int main()
{
    int n;
    std::cin >> n;
    std::vector<int> nums(n + 1);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> nums[i];
    }
    std::vector<int> res;
    for (const int &num : nums)
    {
        auto it = std::lower_bound(res.begin(), res.end(), num);
        if (it == res.end())
        {
            res.push_back(num);
        }
        else
        {
            *it = num;
        }
    }
    std::cout << res.size();
    return 0;
}