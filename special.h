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

template<typename ostrm>
ostrm& operator<<(ostrm& os,const compbone& c)
{
	return os<<c.pivot;
}

struct color
{
	track<common_types::vector3> c,a;
};

struct texture
{
	common::texture t;
	std::string filename;
};
template<typename ostrm>
ostrm& operator<<(ostrm& os,const texture& c)
{
	return os<<c.t<<'\t'<<c.filename;
}

using texture_weight = track<std::uint16_t>;

}