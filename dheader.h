#pragma once
#include"dh.h"

namespace m2
{
struct dheader
{
	version ver;
	offset name;
	md20_flags flags;

	offset loops;
	offset sequences;
	offset sequences_lookups;
	offset bones;
	offset key_bone_lookups;
	offset vertices;
	std::uint32_t num_skin_profiles;
	offset colors;
	offset textures;
	offset texture_weights;
	offset texture_transforms;
	offset replacable_texture_lookup;
	offset materials;

	offset bone_lookup_table;
	offset texture_lookup_table;
	offset tex_unit_lookup_table;
	offset transparency_lookup_table;
	offset texture_transforms_lookup_table;

	common_types::aa_box bounding_box;
	float bounding_sphere_radius;
	common_types::aa_box collision_box;
	float collision_sphere_radius;
	offset collision_triangles;
	offset collision_vertices;
	offset collision_normals;
	offset attachments;

	offset attachment_lookup_table;
	offset events;
	offset lights;
	offset cameras;
	offset camera_lookup_table;
	offset ribbon_emitters;
	offset particle_emitters;
	offset texture_combiner_combos;
};
}