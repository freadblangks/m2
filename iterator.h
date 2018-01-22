#pragma once
#include<string>
#include<cstddef>
#include<iterator>

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
		return reinterpret_cast<pointer>(str.data()+pos)[p];
	}
	m2_iterator& operator+=(difference_type p)
	{
		pos+=sizeof(value_type)*p;
		return *this;
	}
	reference operator*() const
	{
		return *reinterpret_cast<pointer>(str.data()+pos);
	}
	pointer operator->() const
	{
		return reinterpret_cast<pointer>(str.data()+pos);
	}
	m2_iterator& operator-=(difference_type p)
	{
		pos-=sizeof(value_type)*p;
		return *this;
	}
	m2_iterator& operator++()
	{
		pos+=sizeof(value_type);
		return *this;
	}
	auto operator++(int)
	{
		auto temp(*this);
		++*this;
		return temp;
	}
	auto operator--(int)
	{
		auto temp(*this);
		--*this;
		return temp;
	}	
};

template<typename T>
auto operator+(m2_iterator<T> a,std::size_t b)
{
	return a+=b;
}
template<typename T>
auto operator+(std::size_t b,m2_iterator<T> a)
{
	return a+=b;
}
template<typename T>
auto operator-(m2_iterator<T> a,std::size_t b)
{
	return a-=b;
}
template<typename T>
std::ptrdiff_t operator-(const m2_iterator<T> &a,const m2_iterator<T> &b)
{
	return (a.pos-b.pos)/sizeof(T);
}

}