#include "Set.h"
using namespace std;
#include <iostream>

Set::Set()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
	
}

Set::Set(const Set & other)
{
	m_size = 0;
	head = nullptr;
	tail = head;
	Node*p = other.head;
	while(p!=nullptr) {
		insert(p->m_value);	//loops through to insert data
		p = p->m_next;
	}
}

Set::~Set()
{
	Node * p = head;
	while (p != nullptr) {
		Node * temp = p->m_next;
		delete p;
		p = temp;
	}
}


Set& Set::operator= (const Set & s)
{
	if (this != &s) {
		Set temp (s);
		swap(temp);
	}
	return *this;
}


bool Set::empty() const
{
	return (m_size == 0);
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{

	//if the value is already in the list, return false

	if (contains(value))
		return false;
	if (empty()) {

		//add value to the front if set is empty
		Node * p = new Node;
		p->m_value = value;
		p->m_next = nullptr;
		p->m_prev = nullptr;
		head = p;
		tail = p;
		m_size++;
		return true;
	}
	else if (value < head->m_value) {
		//add value to the front if it belongs there alphabetically 
		Node * p;
		p = new Node; //allocate new node
		p->m_value = value; //assign value
		p->m_next = head; //next points to old first value
		p->m_prev = nullptr; //since it's now the first item
		head->m_prev = p; //old prev points to new first value
		head = p; //head now points to the new first value

		m_size++;
		return true;
	}
	else if (value > tail->m_value) {//@ end
		Node * n = new Node; //allocate new node
		n->m_value = value;
		n->m_next = nullptr; //assign new next...
		n->m_prev = tail; //...and prev
		tail->m_next = n;
		tail = n;
		m_size++;
		return true;
	}
	else {
		Node * p = head;
		while (p->m_next != nullptr) {
			//traverses set
			if (value >= p->m_value && value <= p->m_next->m_value) {
				break; //p now points right above the position I want!
			}

			p = p->m_next; //move to next node

		}
		if (p->m_next != nullptr) { //neither at beginning nor end 
			Node * n = new Node; //allocate new node
			n->m_value = value; //assign value
			n->m_next = p->m_next; //assign new next...
			n->m_prev = p; //...and prev
			Node * nn = p->m_next;
			p->m_next = n; //update p's next...
			nn->m_prev = n; //and the prev for the node after
			m_size++;
			return true;
		}
	}
	return false;
}

bool Set::erase(const ItemType& value)
{
	
	if (empty()) {
		return false; //there's nothing to delete
	}
	if (!contains(value)) {
		return false;
	}

	//case#1: erasing the first node
	if (head->m_value == value) { 
		Node* eraseThis = head; //create temporary pointer to remember the location
		if (m_size == 1) {
			head = nullptr;
			tail = nullptr;
			delete eraseThis;
			m_size--;
			return true;
		}
		head = eraseThis->m_next; //update position of head
		head->m_prev = nullptr;
		delete eraseThis;
		m_size--;
		return true; 
	}
	else if (tail->m_value == value) {//erase at the end
		Node * eraseThis = tail;
		tail = eraseThis->m_prev; //update position of tail
		tail->m_next = nullptr;
		delete eraseThis;
		m_size--;
		return true;
	}
		//case#2: erasing any other node
		//loop through and find value
		Node *p = new Node;
		while (p != nullptr) {
			if (p->m_next != nullptr && p->m_next->m_value == value) //node following p AND that following node has the value we want
				break; //p now points to the node above the one we want
			p = p->m_next;
		}
		if (p != nullptr) { //found!
			Node * eraseThis = p->m_next; //eraseThis points to the Node we want to delete
			p->m_next = eraseThis->m_next; //update position of p's next
			eraseThis->m_next->m_prev = p; //update the prev of the Node after eraseThis
			delete eraseThis;
			m_size--;
			return true;
		}
	return false; //if it didn't find anything
}

bool Set::contains(const ItemType& value) const
{
	Node * p = head;
	while (p != nullptr) {
		if (p->m_value == value)
			return true;
		p = p->m_next;
	}
	return false;
}

bool Set::get(int pos, ItemType& value) const
{
	if (pos >= 0 && pos < m_size) {
		Node * p = head;
		for (int index = 0; index < pos; index++) {
			p = p->m_next;
		}
		value = p->m_value;
		return true;
	}
	return false;
	
}

void Set::swap(Set& other)
{


	//swap sizes
	int tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;

	//swap heads
	Node * tempHead = head;
	head = other.head;
	other.head = tempHead;

	//swap tails
	Node * tempTail = tail;
	tail = other.tail;
	other.tail = tempTail;
	
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set united;
	Set s1Copy(s1);
	Set s2Copy(s2);
	ItemType temp;
	
	for (int i = 0; i < s1Copy.size(); i++) {
		s1Copy.get(i, temp);
		united.insert(temp);
	}
	for (int j = 0; j < s2Copy.size(); j++) {
		s2Copy.get(j, temp);
		united.insert(temp);
	}
	result = united;
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	Set s1Copy(s1);
	Set s2Copy(s2);
	Set subtracted;
	ItemType temp;
	for (int i = 0; i < s1Copy.size(); i++) {
		s1Copy.get(i, temp);
		if (!s2Copy.contains(temp))
			subtracted.insert(temp); //if s2 DOES NOT contain that value, add it to subtracted
									 //if s2 DOES contain that value, nothing happens and loop continues
	}
	result = subtracted;
}


void Set::dump() const {
	Node* n = head;

	cerr << "forward linking: " << endl;
	while (n != nullptr) {
		cerr << n->m_value << " ";
		n = n->m_next;
	}
	cerr<<endl;
	n = tail;
	cerr << "backward linking: " << endl;
	while (n != nullptr) {
		cerr << n->m_value << " ";
		n = n->m_prev;
	}
	cerr << endl;
}