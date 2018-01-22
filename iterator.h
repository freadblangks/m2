#pragma once
#include<string>
#include<cstddef>

namespace m2
{
template<typename T>
struct m2_iterator
{
	using value_type = T;
	using pointer = value_type*;
	using reference = T&;
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::size_t;
	std::string &str;
	std::size_t pos;
	m2_iterator(std::string &s,std::size_t p):str(s),pos(p){}
	reference operator[](difference_type p) const
	{
		return reinterpret_cast<pointer>(str.data())[p];
	}
};
}