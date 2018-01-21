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

struct texture_transform
{
	track<common_types::vector3> translation;
	track<common_types::vector4> rotation;
	track<common_types::vector3> scaling;
};

struct attachment
{
	common::attachment t;
	track<std::uint8_t> animate_attached;
};
template<typename ostrm>
ostrm& operator<<(ostrm& os,const attachment& c)
{
	return os<<c.t;
}

struct event
{
	common::event t;
	track_base enabled;
};

struct light
{
	common::light t;
	track<common_types::vector3> ambient_color;
	track<float> ambient_intensity;
	track<common_types::vector3> diffuse_color;
	track<float> diffuse_intensity;
	track<float> attenuation_start;
	track<float> attenuation_end;
	track<std::uint8_t> visibility;
};

}