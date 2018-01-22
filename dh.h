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

struct fake_animation_block_base
{
	offset<offset<std::uint32_t>> timestamps;
};
template<typename T>
struct fake_animation_block:fake_animation_block_base
{
	offset<offset<T>> values;
	using value_type = T;
	using pointer = value_type*;
	using reference = T&;
};

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

struct particle
{
	common::particle t;
	offset<char> geometry_model_filename;    // if given, this emitter spawns models
	offset<char> recursion_model_filename;   // if given, this emitter is an alias for the (maximum 4) emitters of the given model

	std::uint8_t blending_type;                       // A blending type for the particle. See Below
	std::uint8_t emitter_type;                        // 1 - Plane (rectangle), 2 - Sphere, 3 - Spline, 4 - Bone
	std::uint16_t particleColor_index;                // This one is used for ParticleColor.dbc. See below.

	std::array<std::uint8_t,2> multi_texture_param_x;
	std::uint16_t texture_tile_rotation;               // Rotation for the texture tile. (Values: -1,0,1) -- priorityPlane
	std::uint16_t texture_dimensions_rows;            // for tiled textures
	std::uint16_t texture_dimensions_columns;
	track<float> emission_speed;             // Base velocity at which particles are emitted.
	track<float> speed_variation;            // Random variation in particle emission speed. (range: 0 to 1)
	track<float> vertical_range;             // Drifting away vertically. (range: 0 to pi) For plane generators, this is the maximum polar angle of the initial velocity; 
									// 0 makes the velocity straight up (+z). For sphere generators, this is the maximum elevation of the initial position; 
									// 0 makes the initial position entirely in the x-y plane (z=0).
	track<float> horizontal_range;           // They can do it horizontally too! (range: 0 to 2*pi) For plane generators, this is the maximum azimuth angle of the initial velocity; 
									// 0 makes the velocity have no sideways (y-axis) component. 
									// For sphere generators, this is the maximum azimuth angle of the initial position.
	track<float> gravity;                   // Not necessarily a float; see below.
	track<float> lifespan;
	float lifespanVary;                       // An individual particle's lifespan is added to by lifespanVary * random(-1, 1)
	track<float> emission_rate; 
	float emission_rate_vary;                   // This adds to the base emissionRate value the same way as lifespanVary. The random value is different every update.
	track<float> emission_area_length;        // For plane generators, this is the width of the plane in the x-axis.
									// For sphere generators, this is the minimum radius.
	track<float> emission_area_width;         // For plane generators, this is the width of the plane in the y-axis.
									// For sphere generators, this is the maximum radius.
	track<float> z_source;                   // When greater than 0, the initial velocity of the particle is (particle.position - C3Vector(0, 0, zSource)).Normalize()
	fake_animation_block<common_types::vector3> color_track;             // Most likely they all have 3 timestamps for {start, middle, end}.
	fake_animation_block<std::uint16_t> alpha_track;
	fake_animation_block<common_types::vector2> scale_track;

	common_types::vector2 scale_vary;                      // A percentage amount to randomly vary the scale of each particle
	fake_animation_block<std::uint16_t> head_cell_track;             // Some kind of intensity values seen: 0,16,17,32 (if set to different it will have high intensity)
	fake_animation_block<std::uint16_t> tail_cell_track;

	common::particle_middle pm;

	track<common_types::vector3> spline_points;                                  //Set only for spline praticle emitter. Contains array of points for spline
	track<std::uint8_t> enabledIn;                 // (boolean) Appears to be used sparely now, probably there's a flag that links particles to animation sets where they are enabled.
	std::array<std::array<std::uint16_t,2>,2> multi_texture_params;
};

}
}