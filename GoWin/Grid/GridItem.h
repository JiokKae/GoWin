#pragma once
#include <string>
#include <vector>

class GridItem
{
public:
	enum class State
	{
		Null,
		Stone,
		Wall,
		TempStone,
	};

public:
	GridItem(int x, int y, State state);

	int x() const;
	int y() const;
	State state() const;

	virtual operator std::string() const;
	friend std::ostream& operator<<(std::ostream&, const GridItem&);

protected:
	using value_array = std::vector<std::pair<std::string, std::string>>;
	virtual value_array values() const;

private:
	int m_x;
	int m_y;
	State m_state = State::Null;
};
