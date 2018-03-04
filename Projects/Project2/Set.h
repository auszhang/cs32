#ifndef SET_H
#define SET_H
#include <string>

typedef std::string ItemType;

class Set
{
public:
	Set();
	Set(const Set & other);
	~Set();
	Set& operator= (const Set & s);

	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	
	void dump() const;
		
	void swap(Set& other);
private:
	struct Node
	{
		ItemType m_value;
		Node * m_next;
		Node * m_prev;
	};
	Node * head;
	Node * tail;
	int m_size;
};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);

#endif