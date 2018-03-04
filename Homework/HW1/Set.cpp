#include "Set.h"

Set::Set() 
{
	// Create an empty set.
	m_size = 0;
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
	if (m_size > DEFAULT_MAX_ITEMS)
		return false;
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

bool Set:: erase(const ItemType& value)
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
	for (int  j= 0; j < size(); j++) {
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
	ItemType tempArray[DEFAULT_MAX_ITEMS]; 
	
	//switch sizes
	tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;
	
	//switch array contents
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++) {
		tempArray[i] = m_set[i];
		m_set[i] = other.m_set[i];
		other.m_set[i] = tempArray[i];
	}
}
