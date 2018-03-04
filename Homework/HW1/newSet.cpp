#include "newSet.h"
#include <iostream>
using namespace std;

Set::Set()
{
	// default
	m_size = 0;
	m_capacity = DEFAULT_MAX_ITEMS;
	m_set = new ItemType[DEFAULT_MAX_ITEMS];
}

Set::Set(int capacity)
{
	//takes an integer parameter
	if (capacity < 0) {
		cout << "Error: the capacity of the set must be positive." << endl;
		exit(1);
	}
	m_size = 0;
	m_capacity = capacity;
	m_set = new ItemType[capacity];
}

Set::Set(const Set & copySet)
{ 
	//copy constructor
	m_set = new ItemType[m_capacity];
	m_size = copySet.m_size;
	m_capacity = copySet.m_capacity;

	for (int i = 0; i < m_size; i++)
		m_set[i] = copySet.m_set[i];
}

Set::~Set()
{
	//destructor
	delete [] m_set;
}	

Set& Set::operator= (const Set s)
{
	//assignmenet operator
	if (this != &s)
		Set temp = s;
	
	return *this;
}


bool Set::empty() const
{
	if (m_size > 0)
		return false;

	return true;
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{
	if (m_size < m_capacity) {
		//checks if value is already present
		for (int i = 0; i < size(); i++) {
			if (value == m_set[i])
				return false;
		}

		//inserts value and returns true
		m_set[m_size] = value;
		m_size++;
		return true;
	}
	return false;
}

bool Set::erase(const ItemType& value)
{
	bool result = false;
	for (int i = 0; i < size(); i++) {
		if (value == m_set[i]) {
			for (int j = i; j < size(); j++)
				m_set[j] = m_set[j + 1];
			m_size--;
			result = true;
		}
	}
	return result;
}

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < size(); i++) {
		if (value == m_set[i])
			return true;
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	ItemType sortedArray[DEFAULT_MAX_ITEMS];
	int counter = 0;
	ItemType placeholder;

	//sorts the original set
	for (int j = 0; j < size(); j++) {
		counter = 0;
		for (int k = 0; k < size(); k++) {
			if (m_set[j] > m_set[k])
				counter++;
		}
		sortedArray[counter] = m_set[j];
	}

	//finds the desired value
	if (i >= 0 && i < size()) {
		value = sortedArray[i];
		return true;
	}
	return false;
}

void Set::swap(Set& other)
{
	//create temporary array and temporary size for switching
	int tempSize;
	int tempCapacity;
	ItemType * ptr;
	
	//switch sizes
	tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;
	
	//switch capacities
	tempCapacity = m_capacity;
	m_capacity = other.m_capacity;
	other.m_capacity = tempCapacity;

	//switch array contents
	ptr = m_set;
	m_set = other.m_set;
	other.m_set = ptr;

}