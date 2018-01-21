#pragma once

#include"track.h"

namespace m2
{
struct compbone:common::compbone
{
	track<vector3> translation;
	track<vector4> rotation;
	track<vector3> scale;
	vector3 pivot;
};
}