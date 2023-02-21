#pragma once
#include <vector>

template<typename T>
class Node {
public:
	Node(const T& data);
	Node(T&& data);

	const T& data() const;
private:
	T m_data;
	std::vector<Node<T>> m_childs;
};

template<typename T>
inline Node<T>::Node(const T& data)
	: m_data(data)
{
}

template<typename T>
inline Node<T>::Node(T&& data)
	: m_data(data)
{
}

template<typename T>
inline const T& Node<T>::data() const
{
	return m_data;
}
