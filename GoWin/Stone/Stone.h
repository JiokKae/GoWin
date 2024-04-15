#pragma once

#include "../stdgo.h"
#include "../Grid/GridItem.h"

class Stone : public GridItem
{
public:
	Stone( int x, int y, int sequence, Color color );
	Stone( int x, int y, int sequence, std::string temp );
	Stone(); 			// 빈 돌
	Stone( std::string Wall );	// 벽

	Color color() const;
	int sequence() const;
	bool is_capturer() const;
	void set_capturer(bool capturer);

	virtual operator std::string() const override;
private:
	virtual value_array values() const override;

private:
	int m_sequence = 0;
	Color m_color = Color::None;
	bool m_capturer = false;
};
