/*#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Set s;
	assert(s.empty());
	s.insert("hi");
	assert(s.size()==1);
	assert(!s.empty());
	s.insert("bye");
	assert(s.size() == 2);
	assert(!s.empty());
	s.insert("x");
	assert(s.size() == 3);
	assert(!s.empty());
	assert(!s.erase("hai"));
	assert(s.erase("bye"));
	assert(s.size() ==2);
	assert(s.erase("hi"));
	assert(s.size() == 1);
	assert(s.erase("x"));
	assert(s.empty());

	Set ss;
	ss.insert("uh oh");
	ss.erase("uh oh");
	assert(ss.empty());

	Set s1;
	
	ItemType value;
	s1.insert("d");
	s1.insert("a");
	s1.insert("e");
	s1.insert("b");
	s1.insert("c");
	assert(s1.size() == 5);
	s1.get(0, value);

	Set a;
	a.insert("2");
	a.insert("8");
	a.insert("3");
	a.insert("9");
	a.insert("5");
	Set b;
	b.insert("6");
	b.insert("3");
	b.insert("8");
	b.insert("5");
	b.insert("10");
	
	assert(a.contains("2") && a.contains("8") && a.contains("5") && a.contains("9"));

	Set result;
	result.insert("45");
	unite(a, b, result);
	assert(!result.contains("45"));
	
	subtract(a, b, result);
	assert(a.contains("2") && a.contains("9"));
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}
*/