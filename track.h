#pragma once

#include"common.h"

namespace m2
{
struct track_base:common::track
{
	std::vector<std::vector<std::uint32_t>> timestamps;
};

template<typename T>
struct track:track_base
{
	std::vector<std::vector<T>> values;
};
}