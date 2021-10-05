/*
    @@ 反转数字
    @@ auther pengxd
    @@ date: 2020/11/14
*/

#include<iostream>

class Solution {
public:
    Solution(int x_) : x(x_) {}
    int reverse() {
        int flag = x>0?1:-1;
        x = flag > 0 ? x : -x;
        int so,pop = 0;
        while(x == 0)
        {
            so = x%10;
            pop = (so + pop)*10;
            x = x / 10;
        }
        pop = pop*flag;
        return ((pop > 2147483647)||(pop < -2147483648)) ? 0 : pop;
    }
private:
    int x;
};

int main()
{
    Solution  a(123);
    std::cout << a.reverse() << std::endl;
    return 0;
}
