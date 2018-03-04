#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Set uls;
	assert(uls.insert(20));
	assert(uls.insert(10));
	assert(uls.size() == 2);
	assert(uls.contains(10));
	ItemType x = 30;
	assert(uls.get(0, x) && x == 10);
	assert(uls.get(1, x) && x == 20);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}