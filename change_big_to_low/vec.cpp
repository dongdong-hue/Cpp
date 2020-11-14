#include<iostream>
#include<map>
#include<vector>

using namespace std;
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        typedef map<int, int>  unique;
        unique kv;
        unique resul;
        for (int i = arr[0]; i < arr.size() ; ++i)
        {
            kv[arr[i]]++;
        }
        unique::iterator pos;
        for (pos = kv.begin(); pos != kv.end(); ++pos)
        {
            if(resul[pos->second] == 1)
            return false;
            resul[pos->second]++;
        }
        return true;
    }
};

int main()
{
	Solution sol;
	vector<int> arr={1,1,2,2,2,3};
	cout << sol.uniqueOccurrences(arr) << endl;
//   	std::map<int, int> kv;
//	std::cout << kv[1] << std::endl; 
	return 0;
}
