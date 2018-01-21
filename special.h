#pragma once

#include"track.h"
#include"common.h"

namespace m2
{
struct compbone
{
	common::compbone c;
	track<common_types::vector3> translation;
	track<common_types::vector4> rotation;
	track<common_types::vector3> scale;
	common_types::vector3 pivot;
};
}