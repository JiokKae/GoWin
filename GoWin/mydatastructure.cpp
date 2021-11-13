#include "mydatastructure.h"

void NodeLinkManager::print()
{
	Node* np = m_head;

	while (np != nullptr)
	{
		np->data().print(std::wcout);
		std::cout << "-->";
		np = np->link();
	}
	std::cout << std::endl;
}

Node::~Node()
{
	if (m_link != nullptr)
	{
		m_link->~Node();
		m_link = nullptr;
	}
	m_child.~NodeLinkManager();
}

void Node::print()
{
	m_data.print(std::wcout);
}

void LinkedList::print() const
{
	Node* np = m_head;
	while (np != nullptr)
	{
		np->print();
		std::cout << "--------" << std::endl;
		np = np->next();
	}
	std::cout << std::endl;
}
