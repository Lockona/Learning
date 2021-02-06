#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		int i = 0;
		vector<int> temp(2,-1);
		for (i = 0; i < nums.size(); i++)
		{
			if (nums[i] + nums[j] == target) 
			{
				temp[0] = i;
				temp[1] = j;
				return temp;
			}
		}
		return temp;
	}
};
int main() {
	int n;
	vector<int> nums(10);
	for (n = 0; n < 10; n++) 
	{
		nums[n] = n;
	}
	cin >> n;
	Solution s;
	nums=s.twoSum(nums, n);
	cout << nums[0] << endl;
	cout << nums[1] << endl;
	return 0;
}