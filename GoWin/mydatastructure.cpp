#include "mydatastructure.h"

LRESULT Node::init()
{

	return LRESULT();
}

void Node::release()
{
}

void Node::print()
{
	print_data(m_data);
}

void NodeLinkManager::print()
{
	Node* np = m_head;

	while (np != nullptr)
	{
		print_data(np->data());
		cout << "-->";
		np = np->link();
	}
	cout << endl;
}