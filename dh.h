#pragma once
#include"common.h"

namespace m2
{
template<typename T>
struct offset
{
	std::uint32_t number;
	std::uint32_t offset_elements;
	using value_type = T;
	using pointer = value_type*;
};

namespace dh
{
struct track_base
{
	common::track t;
	offset<offset<std::uint32_t>> timestamps;
};
template<typename T>
struct track:track_base
{
	offset<offset<T>> values;
	using value_type = T;
	using pointer = value_type*;
	using reference = T&;
};

struct compbone
{
	common::compbone c;
	track<common_types::vector3> translation;
	track<std::array<std::uint16_t,4>> rotation;
	track<common_types::vector3> scale;
	common_types::vector3 pivot;
};
struct color
{
	track<common_types::vector3> c;
	track<std::uint16_t> a;
};
struct texture
{
	common::texture t;
	offset<char> filename;
};
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

struct camera
{
	common::camera t;
	track<spline_key<common_types::vector3>> positions;
	common_types::vector3 position_base;
	track<spline_key<common_types::vector3>> target_position;
	common_types::vector3 target_position_base;
	track<spline_key<float>> roll;
	track<spline_key<float>> fov;
};

struct ribbon
{
	common::ribbon t;
	offset<std::uint16_t> texture_indices;
	offset<std::uint16_t> material_indices;
	track<common_types::vector3> color;
	track<std::uint16_t> alpha;
	track<float> height_above;
	track<float> height_below;
	float edge_per_second;
	float edge_life_time;
	float gravity;
	std::uint16_t texture_rows,texture_cols;
	track<std::uint16_t> texture_slot;
	track<std::uint8_t> visibility;
	std::int16_t priority_plane;
	std::uint16_t padding;
};

}
}