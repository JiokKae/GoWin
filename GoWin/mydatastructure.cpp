#include "mydatastructure.h"
#include "PlacementInfo.h"

LRESULT Node::init()
{

	return LRESULT();
}

void Node::release()
{
}

void Node::print()
{
	m_data->print();
}

void NodeLinkManager::print()
{
	Node* np = m_head;

	while (np != nullptr)
	{
		np->data()->print();
		cout << "-->";
		np = np->link();
	}
	cout << endl;
}