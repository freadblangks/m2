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

struct camera
{
	common::camera t;
	track positions;
	common_types::vector3 position_base;
	track target_position;
	common_types::vector3 target_position_base;
	track roll;
	track fov;
};

struct ribbon
{
	common::ribbon t;
	offset texture_indices;
	offset material_indices;
	track color;
	track alpha;
	track height_above;
	track height_below;
	float edge_per_second;
	float edge_life_time;
	float gravity;
	std::uint16_t texture_rows,texture_cols;
	track texture_slot;
	track visibility;
	std::int16_t priority_plane;
	std::uint16_t padding;
};

}
}