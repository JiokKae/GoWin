#include "Stone.h"
#include <format>

Stone::Stone( int x, int y, int sequence, Color color )
	: GridItem(x, y, State::Stone)
	, m_sequence( sequence )
	, m_color( color )
{
}

Stone::Stone( int x, int y, int sequence, std::string temp ) 
	: GridItem(x, y, State::TempStone)
	, m_sequence( sequence )
{
}

Stone::Stone()
	: GridItem(1, 1, State::Null)
{
}

Stone::Stone( std::string Wall )
	: GridItem(0, 0, State::Wall)
{
}

Stone::operator std::string() const
{
	return std::format("Stone({}, {})", x(), y());
}

Stone::value_array Stone::values() const
{
	value_array stone_values{ GridItem::values() };
	stone_values.emplace_back("sequence", std::to_string(m_sequence));
	stone_values.emplace_back("color", std::to_string((int)m_color));
	stone_values.emplace_back("killer", std::to_string(m_killer));

	return stone_values;
}
