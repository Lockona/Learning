#include <iostream>
#include <vector>
#include <string>

using namespace std;

 struct ListNode {
     int val;
     ListNode *next;
     ListNode() : val(0), next(nullptr) {}
	 ListNode(int x) : val(x), next(nullptr) {}
     ListNode(int x, ListNode *next) : val(x), next(next) {}
 };

class Solution {
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		ListNode* temp1 = new ListNode,*temp2 = nullptr;
		temp2 = temp1;
		int sum = 0, carry = 0;
		while (l1 || l2)
		{
			int n1 = l1 ? l1->val : 0;
			int n2 = l2 ? l2->val : 0;
			sum = n1 + n2 + carry;
			carry = sum / 10;
			temp2->val = sum % 10;
			if (l1)
				l1 = l1->next;
			if (l2)
				l2 = l2->next;
			if (l1 || l2) {
				temp2->next = new ListNode(0);
				temp2 = temp2->next;
			}
		}
		if (carry > 0) {
			temp2->next = new ListNode(0);
			temp2 = temp2->next;
			temp2->val = carry;
			temp2->next = NULL;
		}
		else {
			temp2->next = NULL;
		}
		return temp1;
	}
};
int main() {
	ListNode* s1 = new ListNode[3];
	ListNode* s2 = new ListNode[3];
	ListNode* s3;
	s1[0].val = 2;
	s1[0].next = &s1[1];
	s1[1].val = 4;
	s1[1].next = &s1[2];
	s1[2].val = 3;
	s1[2].next = NULL;
	s2[0].val = 5;
	s2[0].next = &s2[1];
	s2[1].val = 6;
	s2[1].next = NULL;
	Solution s;
	s3 = s.addTwoNumbers(s1, s2);
	while (s3 != NULL) {
		cout << s3->val;
		s3 = s3->next;
	}
	delete s1;
	delete s2;
	delete s3;
	return 0;
}