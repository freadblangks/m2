#pragma once
#include<array>

namespace m2
{

union magic
{
	std::uint32_t i;
	std::array<char,4> a;
};
}