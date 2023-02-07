#include "mydatastructure.h"

Node::~Node()
{
	if (m_link != nullptr)
	{
		m_link->~Node();
		m_link = nullptr;
	}
}

const PlacementInfo& Node::data() const
{
	return m_data;
}

const std::vector<Node>& Node::childs() const
{
	return m_childs;
}

void Node::print()
{
	std::wcout << m_data;
}
