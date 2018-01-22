#pragma once
#include"dh.h"

namespace m2
{
namespace dh
{
struct dheader
{
	version ver;
	offset<char> name;
	md20_flags flags;

	offset<loop> loops;
	offset<sequence> sequences;
	offset<std::uint16_t> sequences_lookups;
	offset<dh::compbone> bones;
	offset<std::uint16_t> key_bone_lookups;
	offset<vertex> vertices;
	std::uint32_t num_skin_profiles;
	offset<dh::color> colors;
	offset<dh::texture> textures;
	offset<dh::texture_weight> texture_weights;
	offset<dh::texture_transform> texture_transforms;
	offset<std::uint16_t> replacable_texture_lookup;
	offset<material> materials;

	offset<std::uint16_t> bone_lookup_table;
	offset<std::uint16_t> texture_lookup_table;
	offset<std::uint16_t> tex_unit_lookup_table;
	offset<std::uint16_t> transparency_lookup_table;
	offset<std::uint16_t> texture_transforms_lookup_table;

	common_types::aa_box bounding_box;
	float bounding_sphere_radius;
	common_types::aa_box collision_box;
	float collision_sphere_radius;
	offset<std::uint16_t> collision_triangles;
	offset<common_types::vector3> collision_vertices;
	offset<common_types::vector3> collision_normals;
	offset<dh::attachment> attachments;

	offset<std::uint16_t> attachment_lookup_table;
	offset<dh::event> events;
	offset<dh::light> lights;
	offset<dh::camera> cameras;
	offset<std::uint16_t> camera_lookup_table;
	offset<dh::ribbon> ribbon_emitters;
	offset<void> particle_emitters;
	
	offset<std::uint16_t> texture_combiner_combos;
};
}
}