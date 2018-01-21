#pragma once
#include"common.h"

namespace m2
{
struct offset
{
	std::uint32_t number;
	std::uint32_t offset_elements;
};

namespace dh
{
struct track_base
{
	common::track t;
	offset timestamps;
};
struct track:track_base
{
	offset values;
};

struct compbone
{
	common::compbone c;
	track translation;
	track rotation;
	track scale;
	common_types::vector3 pivot;
};
struct color
{
	track c,a;
};
struct texture
{
	common::texture t;
	offset filename;
};
using texture_weight = track;

struct texture_transform
{
	track translation;
	track rotation;
	track scaling;
};

struct attachment
{
	common::attachment t;
	track animate_attached;
};

struct event
{
	common::event t;
	track_base enabled;
};

struct light
{
	common::light t;
	track ambient_color;
	track ambient_intensity;
	track diffuse_color;
	track diffuse_intensity;
	track attenuation_start;
	track attenuation_end;
	track visibility;
};

}
}