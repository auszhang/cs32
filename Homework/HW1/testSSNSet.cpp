#include "SSNSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;


int main()
{
	SSNSet s;
	assert(s.size() == 0);
	s.add(123456789);
	assert(s.size() == 1);
	s.add(987654321);
	s.add(192837461);
	assert(s.size() == 3);
	s.print();
	cout << "All tests passed." << endl;
}
