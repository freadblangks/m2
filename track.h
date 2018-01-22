#pragma once

#include"common.h"

namespace m2
{
	
struct fake_animation_block_base
{
	std::vector<std::vector<std::uint32_t>> timestamps;
};
template<typename T>
struct fake_animation_block:fake_animation_block_base
{
	std::vector<std::vector<T>> values;
	using value_type = T;
	using pointer = value_type*;
	using reference = T&;
};

struct track_base
{
	common::track t;
	std::vector<std::vector<std::uint32_t>> timestamps;
};

template<typename T>
struct track:track_base
{
	std::vector<std::vector<T>> values;
};
}