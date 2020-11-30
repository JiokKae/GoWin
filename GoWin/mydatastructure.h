#pragma once
#include "framework.h"
using namespace std;

class Node;

class NodeLinkManager
{
	Node* m_head;
	int m_size;

public:
	NodeLinkManager() { m_head = nullptr; m_size = 0; }
	~NodeLinkManager() { m_head = nullptr; m_size = 0; }
	Node* head() { return m_head; }
	int size() { return m_size; }

	void insert_node(Node* node);
	void pick(int index);
	void print();
	void clear();
};

class PlacementInfo;
class Node {
	PlacementInfo* m_data;
	Node* m_link;
	NodeLinkManager m_child;
public:
	Node(PlacementInfo* data) : m_link(nullptr) { this->m_data = data; }
	~Node() { 
		if (m_link != nullptr)
		{
			m_link->~Node();
			m_link = nullptr;
		}
		m_child.~NodeLinkManager();
	}
	LRESULT init();
	void release();

	PlacementInfo* data()		{ return m_data; }
	Node* link()				{ return m_link; }
	Node* next()				{ return m_child.head(); }
	NodeLinkManager& child()	{ return m_child; }

	void set_data(PlacementInfo* data)	{ this->m_data = data; }
	void set_link(Node* link)			{ this->m_link = link; }

	void print(); 
};

inline void NodeLinkManager::insert_node(Node* node)
{
	if (head() == nullptr)
	{
		m_head = node;
	}
	else
	{
		Node* np = m_head;
		while (np->link() != nullptr)
		{
			np = np->link();
		}
		np->set_link(node);

	}
	m_size++;
}

inline void  NodeLinkManager::pick(int index)
{
	if (index < m_size)
		return;

	Node* np = m_head;
	for (int i = 0; i < index; i++)
	{
		np = np->link();
	}
}



inline void NodeLinkManager::clear()
{
	if (m_size == 0)	return;
	if (m_head == nullptr)	return;
	m_head->~Node();
	m_head = nullptr;
	m_size = 0;
}

class LinkedList {
	Node* m_head;
	Node* m_lastNode;
	int m_size;

public:
	LinkedList() { 
		m_head = nullptr; 
		m_lastNode = nullptr;
		m_size = 0; 
	}

	int size() const { return m_size; }

	void push_back(Node* node)
	{
		if (m_head == nullptr)
		{
			m_head = node;
		}
		else
		{
			Node* np = m_head;
			while (np->child().head() != nullptr)
			{
				np = np->next();
			}
			np->child().insert_node(node);
			
		}
		m_lastNode = node;
		m_size++;
	}

	void delete_back()
	{
		if (m_size == 0) return;

		Node* np = m_head;

		// 노드가 1개일 경우
		if (np->next() == nullptr)
		{
			np->~Node();
			m_head = nullptr;
			m_lastNode = nullptr;
			m_size--;
		}
		// 노드가 2개 이상일 경우
		else 
		{
			while (np->next()->next() != nullptr)
			{
				np = np->next();
			}
			m_lastNode = np;
			m_lastNode->next()->~Node();
			m_lastNode->child().clear();
			m_size--;
		}
		
	}

	Node& read(int index) {
		Node* np = m_head;
		for (int i = 0; i < index; i++)
		{
			np = np->next();
		}
		return *np;
	}
	Node& getLastNode() {
		return *m_lastNode;
	}
	void print() {
		Node* np = m_head;
		while (np != nullptr)
		{
			np->print();
			cout << "--------" << endl;
			np = np->next();
		}
		cout << endl;
	}

};
