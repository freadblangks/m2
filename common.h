#pragma once

#include"common_types.h"

namespace m2
{
struct loop
{
	std::uint32_t timestamp;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm &os,const loop &p)
{
	return os<<p.timestamp;
}

struct range
{
	uint32_t minimum;
	uint32_t maximum;
};

struct bounds
{
	common_types::aa_box extent;
	float radius;
};

template<typename T>
struct spline_key
{
	using value_type = T;
	value_type value;
	value_type in;
	value_type out;
};

struct sequence
{
	std::uint16_t id;                   // Animation id in AnimationData.dbc
	std::uint16_t variation_index;       // Sub-animation id: Which number in a row of animations this one is.
	std::uint32_t duration;             // The length of this animation sequence in milliseconds.
	float movespeed;               // This is the speed the character moves with in this animation.
	std::uint32_t flags;                // See below.
	std::int16_t frequency;             // This is used to determine how often the animation is played. For all animations of the same type, this adds up to 0x7FFF (32767).
	std::uint16_t padding;
	range replay;
	std::uint16_t blend_time_in;          // The client blends (lerp) animation states between animations where the end and start values differ. This specifies how long that blending takes. Values: 0, 50, 100, 150, 200, 250, 300, 350, 500.
	std::uint16_t blend_time_out;         // The client blends between this sequence and the next sequence for blendTimeOut milliseconds.
							// For both blendTimeIn and blendTimeOut, the client plays both sequences simultaneously while interpolating between their animation transforms.
	bounds bounds;
	std::int16_t variation_next;         // id of the following animation of this AnimationID, points to an Index or is -1 if none.
	std::uint16_t alias_next;            // id in the list of animations. Used to find actual animation if this sequence is an alias (flags & 0x40)
};
template<typename ostrm>
decltype(auto) operator<<(ostrm &os,const sequence &p)
{
	return os<<p.id;
}


enum class keybone:std::int32_t
{
	larm,rarm,lshoulder,rshoulder,spinelow,waist,head,jaw,rfingerindex,rfingermiddle,rfingerpinky,rfingerring,rfingerthumb,lfingerindex,lfingermiddle,lfingerpinky,lfingerring,lfingerthumb,bth,csr,csl,breath,name,namemount,chd,cch,root,
	not_exist = -1,
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const keybone &c)
{
	os<<std::int32_t(c);
	switch(c)
	{
		case keybone::larm:
			return os<<"(left arm)";
		case keybone::rarm:
			return os<<"(right arm)";
		case keybone::lshoulder:
			return os<<"(left shoulder)";
		case keybone::rshoulder:
			return os<<"(right shoulder)";
		case keybone::spinelow:
			return os<<"(spine low)";
		case keybone::waist:
			return os<<"(waist)";
		case keybone::head:
			return os<<"(head)";
		case keybone::jaw:
			return os<<"(jaw)";
		case keybone::rfingerindex:
			return os<<"(right finger index)";
		case keybone::rfingermiddle:
			return os<<"(right finger middle)";
		case keybone::rfingerpinky:
			return os<<"(right finger pinky)";
		case keybone::rfingerring:
			return os<<"(right finger ring)";
		case keybone::rfingerthumb:
			return os<<"(right finger thumb)";
		case keybone::lfingerindex:
			return os<<"(left finger index)";
		case keybone::lfingermiddle:
			return os<<"(left finger middle)";
		case keybone::lfingerpinky:
			return os<<"(left finger pinky)";
		case keybone::lfingerring:
			return os<<"(left finger ring)";
		case keybone::lfingerthumb:
			return os<<"(left finger thumb)";
		case keybone::bth:
			return os<<"(bth)";
		case keybone::csr:
			return os<<"(csr)";
		case keybone::csl:
			return os<<"(csl)";
		case keybone::breath:
			return os<<"(breath)";
		case keybone::name:
			return os<<"(name)";
		case keybone::namemount:
			return os<<"(name mount)";
		case keybone::chd:
			return os<<"(chd)";
		case keybone::cch:
			return os<<"(cch)";
		case keybone::root:
			return os<<"(root)";
		case keybone::not_exist:
			return os<<"(not exist)";
		default:
			return os<<"(unknown)";
	}
}

struct md20_flags
{
	std::uint32_t flag_tilt_x: 1;
	std::uint32_t flag_tilt_y: 1;
	std::uint32_t : 1;
	std::uint32_t flag_use_texture_combiner_combos: 1;
	std::uint32_t : 1;
	std::uint32_t flag_load_phys_data : 1;
	std::uint32_t : 1;
	std::uint32_t flag_demon_hunter_tatto_glowing: 1;  
	std::uint32_t flag_camera_related: 1;
	std::uint32_t flag_new_particle_record: 1;
	std::uint32_t flag_unk_0x400 : 1;
	std::uint32_t flag_texture_transforms_use_bone_sequences: 1;
	std::uint32_t flag_unk_0x1000: 1;
	std::uint32_t flag_unk_0x2000: 1;
	std::uint32_t flag_unk_0x4000: 1;
	std::uint32_t flag_unk_0x8000: 1;
	std::uint32_t flag_unk_0x10000: 1;
	std::uint32_t flag_unk_0x20000: 1;
	std::uint32_t flag_unk_0x40000: 1;
	std::uint32_t flag_unk_0x80000: 1;
	std::uint32_t flag_unk_0x100000: 1;
	std::uint32_t flag_unk_0x200000: 1;// apparently: use 24500 upgraded model format: chunked .anim files, change in the exporter reordering sequence+bone blocks before name
};

template<typename ostrm>
ostrm& operator<<(ostrm& os,const md20_flags &f)
{
	if(f.flag_demon_hunter_tatto_glowing)
		os<<"[Demon Hunter Tatto Glowing]";
	if(f.flag_unk_0x200000)
		os<<"[Legion 24500 upgraded model format]";
	return os;
}

struct vertex
{
	common_types::vector3 pos;
	std::array<std::uint8_t,4> weight,indice;
	common_types::vector3 normal;
	std::array<common_types::vector2,2> tex_coords;
};

struct material
{
	std::uint16_t flags,blending_mode;
};

namespace common
{
struct compbone
{
	keybone key_bone_id;
	std::uint32_t flags;
	std::int16_t parent_bone;
	std::uint16_t submesh_id;
	struct
	{
		std::uint16_t u_dist_to_furth_desc;
		std::uint16_t u_z_ratio_of_chain;
	}compress_data;
};

struct track
{
	std::uint16_t interpolating;
	std::uint16_t global_sequence;
};

struct texture
{
	std::uint32_t type,flags;
};

template<typename ostrm>
ostrm& operator<<(ostrm& os,const texture& c)
{
	return os<<c.type<<'\t'<<c.flags;
}

struct attachment
{
	std::uint32_t id;
	std::uint16_t bone;
	std::uint16_t unknown;
	common_types::vector3 position;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const attachment &c)
{
	return os<<"id "<<c.id<<"\tbone "<<c.bone<<"\tpivot "<<c.position;
}

struct event
{
	std::uint32_t identifier;
	std::uint32_t data;
	std::uint32_t bone;
	common_types::vector3 position;
};

struct light
{
	std::uint16_t type;
	std::uint16_t bone;
	common_types::vector3 position;
};

struct camera
{
	std::uint32_t type;
	float far_clip;
	float near_clip;
};

struct ribbon
{
	std::uint32_t id;
	std::uint32_t bone_index;
	common_types::vector3 position;
};

struct particle
{
	std::uint32_t id;                        // Always (as I have seen): -1.
	std::uint32_t flags;                             // See Below
	common_types::vector3 position;                       // The position. Relative to the following bone.
	std::uint16_t bone;                              // The bone its attached to.
	std::uint16_t texture;                         // And the textures that are used. 
};

struct particle_middle
{
	std::uint8_t blending_type;                       // A blending type for the particle. See Below
	std::uint8_t emitter_type;                        // 1 - Plane (rectangle), 2 - Sphere, 3 - Spline, 4 - Bone
	std::uint16_t particleColor_index;                // This one is used for ParticleColor.dbc. See below.

	std::array<std::uint8_t,2> multi_texture_param_x;
	std::uint16_t texture_tile_rotation;               // Rotation for the texture tile. (Values: -1,0,1) -- priorityPlane
	std::uint16_t texture_dimensions_rows;            // for tiled textures
	std::uint16_t texture_dimensions_columns;
};

struct particle_last
{
	float mid_point; // Middle point in lifespan (0 to 1).
	std::array<std::array<std::uint8_t,4>,3> color_values; // 3*BGRA
	std::array<float,3> scale_values;
	std::array<std::uint16_t,2> head_cell_begin;
	std::uint16_t unk1; // Always 1
	std::array<std::uint16_t,2> head_cell_end;
	std::uint16_t unk2; // Always 1
	std::array<std::uint16_t,4> tiles; // Indices into the tiles on the texture ? Or tailCell maybe ?
	float tail_length;                         // TailCellTime?
	float twinkle_speed;                       // has something to do with the spread
	float twinkle_percent;                     // has something to do with the spread
	range twinkle_scale;
	float burst_multiplier;                    // ivelScale
	float drag;                               // For a non-zero values, instead of travelling linearly the particles seem to slow down sooner. Speed is multiplied by exp( -drag * t ).
	float base_spin;                           // Initial rotation of the particle quad
	float base_spin_vary;
	float spin;                               // Rotation of the particle quad per second
	float spin_vary;
	common_types::aa_box tumble;
	common_types::vector3 wind_vector;
	float wind_time;
	float follow_speed1;
	float follow_scale1;
	float follow_speed2;
	float follow_scale2;
};

}

}