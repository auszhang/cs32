#include "newSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set s;
	assert(s.empty());
	ItemType x = 9876543;
	assert(!s.get(42, x) && x == 9876543); // x unchanged by get failure
	s.insert(123456789);
	assert(s.size() == 1);
	assert(s.get(0, x) && x == 123456789);

	Set ss;
	ss.insert(12345);
	ss.insert(23456);
	ss.insert(23456);
	ss.insert(34567);
	ss.insert(67890);
	ss.insert(67890);
	ss.insert(56789);
	assert(ss.size() == 5);
	ItemType y;
	ss.get(0, y);
	assert(y == 12345);
	ss.get(4, y);
	assert(y == 67890);
	ss.get(2, y);
	assert(y == 34567);

	Set ss3;
	ss3.insert(12345);
	assert(!ss3.contains(0));
	ss3.insert(23456);
	ss3.insert(0);
	ss3.insert(34567);
	assert(ss3.contains(0));
	ss3.erase(34567);
	assert(ss3.size() == 3 && ss3.contains(12345) && ss3.contains(23456) &&
		ss3.contains(0));
	ItemType r;
	assert(ss3.get(1, r) && r == 12345);
	assert(ss3.get(0, r) && r == 0);

	Set ss1;
	ss1.insert(12345);
	ss1.insert(78901);
	ss1.insert(45678);
	Set ss2;
	ss2.insert(56789);
	ss2.insert(67890);
	ss2.insert(23456);
	ss1.swap(ss2);
	assert(ss1.size() == 3);
	assert(ss1.contains(56789) && ss1.contains(67890) && ss1.contains(23456));
	assert(ss2.size() == 3 && ss2.contains(12345) && ss2.contains(78901) && ss2.contains(45678));

	Set a(1000);   // a can hold at most 1000 distinct items
	Set b(5);      // b can hold at most 5 distinct items
	Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
	ItemType v[6] = { 11111, 33333, 66666, 55555, 99999, 22222 };

	// No failures inserting 5 distinct items into b

	for (int k = 0; k < 5; k++)
		assert(b.insert(v[k]));


	//assert(b.insert(1111));
	//assert(b.insert(3333));
	//assert(b.insert(6666));
	//assert(b.insert(5555));
	//assert(b.insert(9999));

	// Failure if we try to insert a sixth distinct item into b
	assert(!b.insert(v[5]));
	//assert(!b.insert(2222));

	// When two Sets' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(v[5]) && b.insert(v[5]));
	//assert(!a.insert(7777) && b.insert(1010));

	Set negative(-1);

	cout << "Passed all tests" << endl;

}