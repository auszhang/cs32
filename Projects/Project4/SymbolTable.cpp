// SymbolTable.cpp
//Austin Zhang 604736503

#include "SymbolTable.h"
#include <string>
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

// This class does the real work of the implementation.

const int HASH_TABLE_SIZE = 19997;

struct Data {
	Data(string id, int line, int scope) : ID(id), lineNum(line), scopeNum(scope)
	{
		item = nullptr;
	}
	//~Data() { delete item; }
	string ID;
	int lineNum;
	int scopeNum;
	Data * item; //accounting for collisions
};

struct BUCKET {
	BUCKET() {
		data = nullptr;
	}
	//~BUCKET() { delete data; }
	Data * data;
};

class HashTable {
public:
	void insert(const string & id, int lineNum, int scopeNum, Data ** &record);
	bool search(const string& id, int& lineNum, int& scopeNum);
private:
	//vector<Data> m_dataTable;
	unsigned int hashFunc(const string& id) const;
	BUCKET m_buckets[HASH_TABLE_SIZE];
};

void HashTable::insert(const string& id, int lineNum, int scopeNum, Data ** &record) {

	int h = hashFunc(id);
	Data * p = m_buckets[h].data;

	//empty space
	if (p == nullptr) {
		m_buckets[h].data = new Data(id, lineNum, scopeNum);
		record = &m_buckets[h].data;
		return;
	}

	//else...
	Data * d;
	while (p != nullptr) {
		d = p;
		p = p->item;
	}

	d->item = new Data(id, lineNum, scopeNum);
	record = &d->item;
}

bool HashTable::search(const string& id, int& lineNum, int& scopeNum) {
	lineNum = -1;
	int h = hashFunc(id);
	Data * p = m_buckets[h].data;

	while (p != nullptr) {
		if (p->ID == id) {
			lineNum = p->lineNum;
			scopeNum = p->scopeNum;
			//finds line and scope of the item found
		}
		p = p->item;
	}

	if (lineNum == -1)
		return false;
	return true;
}

unsigned int HashTable::hashFunc(const string& id) const {
	unsigned int total = 0;
	hash<string> str_hash;
	unsigned int hashValue = str_hash(id);

	total = hashValue % HASH_TABLE_SIZE;
	return total;
}

class SymbolTableImpl
{
public:
	SymbolTableImpl() : m_scope(0) { m_hashTable = new HashTable; }
	~SymbolTableImpl();
	void enterScope();
	bool exitScope();
	bool declare(const string& id, int lineNum);
	int find(const string& id) const;
private:
	//vector<string> m_ids;
	//vector<int> m_lines;
	HashTable * m_hashTable;
	int m_scope;
	vector<Data**> hashTableData;
};

SymbolTableImpl::~SymbolTableImpl() {
	//free left over memory and free the allocated hashtable
	size_t i = hashTableData.size();
	while (i > 0) {
		i--;
		if (hashTableData[i] != nullptr)
			delete * (hashTableData[i]);
	}
	delete m_hashTable;
}

void SymbolTableImpl::enterScope()
{
	// Extend the id vector with an empty string that
	// serves as a scope entry marker.
	hashTableData.push_back(nullptr);
	m_scope++;
}

bool SymbolTableImpl::exitScope()
{
	// Remove ids back to the last scope entry.
	size_t i = hashTableData.size();
	while (i > 0) {
		i--;
		if (hashTableData[i] == nullptr)
			break;
		delete *(hashTableData[i]);
		*(hashTableData[i]) = nullptr;
		hashTableData.pop_back();
	}
	if (hashTableData.empty())
		return false;

	// Remove the scope entry marker itself.
	hashTableData.pop_back();
	m_scope--;
	return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
	if (id.empty())
		return false;

	// Check for another declaration in the same scope.
	// Return if found, break out if encounter the scope
	// entry marker.

	int scope, line;

	//if found in the same scope 
	if (m_hashTable->search(id, line, scope) && scope == m_scope)
		return false;

	// Save the declaration
	Data ** d;
	m_hashTable->insert(id, lineNum, m_scope, d);
	hashTableData.push_back(d);
	return true;
}

int SymbolTableImpl::find(const string& id) const
{
	if (id.empty())
		return -1;

	// Search back for the most recent declaration still
	// available.
	int line, scope;
	bool result;
	result = m_hashTable->search(id, line, scope);
	if (result == false)
		return -1;
	return line;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
	m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
	delete m_impl;
}

void SymbolTable::enterScope()
{
	m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
	return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
	return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
	return m_impl->find(id);
}
