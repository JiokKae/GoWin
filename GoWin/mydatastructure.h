#pragma once
#include "framework.h"
#include "stdgo.h"

class Node {
	PlacementInfo m_data;
	Node* m_link;
	std::vector<Node> m_childs;
public:
	Node(const PlacementInfo& data)
		: m_data(data)
		, m_link(nullptr)
	{
	}

	~Node();

	const PlacementInfo& data() const;
	Node* link() { return m_link; }
	const std::vector<Node>& childs() const; 

	void set_data(PlacementInfo data) { m_data = data; }
	void set_link(Node* link) { m_link = link; }
	void print();
	
};
