#pramga once

#include"common.h"
#include<string>
#include<vector>

namespace m2
{
namespace runtime
{

struct md20
{
	std::uint32_t magic;
	version vers;
	std::string name;
	md20_flags flags;
	std::vector<loop> loops;
	std::vector<sequence> sequences;
	std::vector<std::uint16_t> sequences_lookups;
	std::vector<compbone> bones;
	std::vector<std::int16_t> key_bone_lookups;
	std::vector<vertex> vertices;
	std::uint32_t num_skin_profiles;
	std::vector<color> colors;
	std::vector<texture> textures;
	std::vector<texture_weight> texture_weights;
	std::vector<texture_transform> texture_transforms;
	std::vector<std::uint16_t> replacable_texture_lookup;
	std::vector<material> materials;

	std::vector<std::uint16_t> bone_lookup_table;
	std::vector<std::uint16_t> texture_lookup_table;
	std::vector<std::uint16_t> tex_unit_lookup_table;
	std::vector<std::uint16_t> transparency_lookup_table;
	std::vector<std::uint16_t> texture_transforms_lookup_table;

	common_types::aa_box bounding_box;
	float bounding_sphere_radius;
	common_types::aa_box collision_box;
	float collision_sphere_radius;
	std::vector<std::uint16_t> collision_triangles;
	std::vector<common_types::vector3> collision_vertices;
	std::vector<common_types::vector3> collision_normals;
	std::vector<attachment> attachments;

	std::vector<std::uint16_t> attachment_lookup_table;
	std::vector<event> events;
	std::vector<light> lights;
	std::vector<camera> cameras;
	std::vector<std::uint16_t> camera_lookup_table;
	std::vector<ribbon> ribbon_emitters;
	std::vector<void> particle_emitters;

	std::vector<std::uint16_t> textureCombinerCombos;
};

}
}
