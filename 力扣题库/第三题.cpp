#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
	int lengthOfLongestSubstring(string s) {
		int i = 0,k = 0;
		int len = 0, t_len = 0;
		string s1;
		if (s.size()) {
			t_len = 1;
			s1.append(&s.at(0), 1);
			for (k = 0; k < s.size()-1; k++) {
				if(k!=0)
					s1.erase(0,1);
				for (i = t_len ; i < s.size(); i++) {
					if (s1.find(s.at(i)) == string::npos)
					{
						s1.append(&s.at(i), 1);	
					}
					else {
						break;
					}
				}
				t_len = i;
				if (t_len - k>= len) {
					len = t_len - k;
				}
			}
		}
		return len;
	}
};
int main() {
	int n;
	string s1 = "";
	Solution s;
	n = s.lengthOfLongestSubstring(s1);
	cout << n << endl;
	return 0;
}