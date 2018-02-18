#include "SSNSet.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet()
{
	// Create an empty SSN set.
	m_ssnsize = 0;
}

bool SSNSet::add(unsigned long ssn)
{
	// Add an SSN to the SSNSet.  Return true if and only if the SSN
	// was actually added.
	if (m_ssnset.insert(ssn)) {
		m_ssnsize++;
		return true;
	}
	return false;
}


int SSNSet::size() const
{
	return m_ssnsize;
}

void SSNSet::print() const
{
	unsigned long value = 0;
	// Write every SSN in the SSNSet to cout exactly once, one per
	// line.  Write no other text.
	for (int i = 0; i < m_ssnsize; i++) {
		m_ssnset.get(i, value);
		cout << value << endl;
	}

}
