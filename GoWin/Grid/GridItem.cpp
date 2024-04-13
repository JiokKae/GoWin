#include "GridItem.h"

#include <format>
#include <iostream>

GridItem::GridItem(int x, int y, State state)
	: m_x{ x }
	, m_y{ y }
	, m_state{ state }
{
}

int GridItem::x() const
{
	return m_x;
}

int GridItem::y() const
{
	return m_y;
}

GridItem::State GridItem::state() const
{
	return m_state;
}

GridItem::operator std::string() const
{
	return std::format("GridItem({}, {})", m_x, m_y);
}

std::ostream& operator<<(std::ostream& out_stream, const GridItem& grid_item)
{
	out_stream << (std::string)grid_item << "----------------" << std::endl;
	for (const auto& value : grid_item.values())
	{
		out_stream << "    " << value.first << ": " << value.second << std::endl;
	}
	out_stream << std::string(std::string(grid_item).size(), '-') << "----------------" << std::endl;

	return out_stream;
}

GridItem::value_array GridItem::values() const
{
	return {
		{ "state", std::to_string((int)m_state),
	} };
}
