#pragma once
#include<cstdint>
#include<array>
#include<chrono>
#include<stdexcept>
#include"file_parser.h"
#include<bitset>
#include"common.h"

namespace m2
{
namespace md20
{
template<typename T>
struct iterator
{
	std::uint32_t number;
	std::uint32_t offset_elements;
	using value_type = T;
};
struct track_base
{
	std::uint16_t interpolation_type;
	std::uint16_t global_sequence;
	iterator<iterator<std::uint32_t>> timestamps;
};

template<typename T>
struct track : track_base
{
	using value_type = T;
	iterator<iterator<value_type>> values;
};

template<typename T>
struct spline_key
{
	using value_type = T;
	value_type value;
	value_type in;
	value_type out;
};

struct color
{
	track<common_types::vector3> color;
	track<std::uint16_t> alpha;
};

struct texture_weight
{
	track<std::uint16_t> weight;
};

struct texture_transform
{
	track<common_types::vector3> translation;
	track<common_types::vector4> rotation;
	track<common_types::vector3> scaling;
};

struct compbone
{
	keybone key_bone_id;
	enum
	{
		spherical_billboard = 0x8,
		cylindrical_billboard_lock_x = 0x10,
		cylindrical_billboard_lock_y = 0x20,
		cylindrical_billboard_lock_z = 0x40,
		transformed = 0x200,
		kinematic_bone = 0x400,       // MoP+: allow physics to influence this bone
		helmet_anim_scaled = 0x1000,  // set blend_modificator to helmetAnimScalingRec.m_amount for this bone
	};
	std::uint32_t flags;
	std::int16_t parent_bone;
	std::uint16_t submesh_id;
	union
	{
		struct
		{
			std::uint16_t u_dist_to_furth_desc;
			std::uint16_t u_z_ratio_of_chain;
		} compress_data;
		std::uint32_t bone_name_crc;         // these are for debugging only. their bone names match those in key bone lookup.
	};
	track<common_types::vector3> translation;
	track<std::array<std::uint16_t,4>> rotation;
	track<common_types::vector3> scale;
	common_types::vector3 pivot;
	void set_pivot(std::uint32_t x,std::uint32_t y,std::uint32_t z)
	{
		pivot.x = x;
		pivot.y = y;
		pivot.z = z;
	}
};



template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const compbone &c)
{
	if(c.key_bone_id==keybone::not_exist)
	{
		if(c.parent_bone==-1)
			os<<"independent bone";
		else
			os<<"->"<<c.parent_bone;
	}
	else
		os<<c.key_bone_id;
	
	return os<<"\tpivot("<<c.pivot<<")";
//			return os<<c.key_bone_id<<' '<<c.parent_bone<<' '<<c.pivot;
}

struct vertex
{
	common_types::vector3 pos;
	std::array<std::uint8_t,4> weight,indice;
	common_types::vector3 normal;
	std::array<common_types::vector2,2> tex_coords;
};

struct texture
{
	std::uint32_t type;
	std::uint32_t flags;
	iterator<char> filename;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const texture &c)
{
	return os<<"type:"<<c.type<<"\tflag:"<<c.flags;
}

struct material
{
	std::uint16_t flags;
	std::uint16_t blending_mode;
};

struct attachment
{
	std::uint32_t id;
	std::uint16_t bone;
	std::uint16_t unknown;
	common_types::vector3 position;
	track<std::uint8_t> animate_attached;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const attachment &c)
{
	return os<<"id "<<c.id<<"\tbone "<<c.bone<<"\tpivot "<<c.position;
}

struct event
{
	union
	{
		std::uint32_t u;
		std::array<char,4> s;
	}identifier;
	std::uint32_t data;
	std::uint32_t bone;
	common_types::vector3 position;
	track_base enabled;
};

struct camera
{
	std::uint32_t type;
	float far_clip;
	float near_clip;
	track<spline_key<common_types::vector3>> target_position;
	common_types::vector3 target_position_base;
	track<spline_key<float>> roll;
	track<spline_key<float>> fov;
};

struct light
{
	std::uint16_t type;
	std::int16_t bone;
	common_types::vector3 position;
	track<common_types::vector3> ambient_color;
	track<float> ambient_intensity;
	track<common_types::vector3> diffuse_color;
	track<float> diffuse_intensity;
	track<float> attenuation_start;
	track<float> attenuation_end;
	track<std::uint8_t> visibility;
};

struct ribbon
{
	std::uint32_t id;
	std::uint32_t bone_index;
	common_types::vector3 position;
	iterator<std::uint16_t> texture_indices;
	iterator<std::uint16_t> material_indices;
	track<common_types::vector3> color_track;
	track<std::uint16_t> alpha_track;
	track<float> height_above_track;
	track<float> height_below_track;
	std::chrono::duration<float> edges_per_second;
	std::chrono::duration<float> edges_lifetime;
	float gravity;
	std::uint16_t texture_rows;
	std::uint16_t texture_cols;
	track<std::uint16_t> tex_slot_track;
	track<std::uint8_t> visibility_track;
	std::int16_t priority_plane;
	std::uint16_t padding;
};

template<typename T>
struct fake_block
{
	std::uint32_t timestamp;
	T value;
	iterator<std::uint32_t> timestamps;
	iterator<common_types::vector3> values;
};

class version
{
	std::uint32_t v;
public:
	decltype(auto) get() const
	{
		return v;
	}
	decltype(auto) get()
	{
		return v;
	}
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const version& v)
{
	auto val(v.get());
	switch(val)
	{
		case 264:
			return os<<"Wrath of the Lich King";
		case 272:
			return os<<"Mists of Pandaria";
		case 273:
			return os<<"Warlords of Draenor";
		case 274:
			return os<<"Legion";
		default:
			if(val<256)
				return os<<"Classic ("<<val<<')';
			else if(260<val&&val<264)
				return os<<"The Burning Crusade ("<<val<<')';
			else if(264<val&&val<272)
				return os<<"Cataclysm ("<<val<<')';
			else
				return os<<"Unknown Expansion ("<<val<<')';
	}
}

struct particle
{
	std::uint32_t particleId;                        // Always (as I have seen): -1.
	std::uint32_t flags;                             // See Below
	common_types::vector3 Position;                       // The position. Relative to the following bone.
	std::uint16_t bone;                              // The bone its attached to.
	std::uint16_t texture;
	iterator<char> geometry_model_filename;    // if given, this emitter spawns models
	iterator<char> recursion_model_filename;   // if given, this emitter is an alias for the (maximum 4) emitters of the given 
	std::uint8_t blendingType;                       // A blending type for the particle. See Below
	std::uint8_t emitterType;                        // 1 - Plane (rectangle), 2 - Sphere, 3 - Spline, 4 - Bone
	std::uint16_t particleColorIndex;                // This one is used for ParticleColor.dbc. See below.
	std::array<std::uint8_t,2> multiTextureParamX;                      // Found below.
	std::uint16_t textureTileRotation;               // Rotation for the texture tile. (Values: -1,0,1) -- priorityPlane
	std::uint16_t textureDimensions_rows;            // for tiled textures
	std::uint16_t textureDimensions_columns;
	track<float> emissionSpeed;             // Base velocity at which particles are emitted.
	track<float> speedVariation;            // Random variation in particle emission speed. (range: 0 to 1)
	track<float> verticalRange;             // Drifting away vertically. (range: 0 to pi) For plane generators, this is the maximum polar angle of the initial velocity; 
	track<float> horizontalRange;           // They can do it horizontally too! (range: 0 to 2*pi) For plane generators, this is the maximum azimuth angle of the initial velocity; 
	track<float> gravity;                   // Not necessarily a float; see below.
	track<float> lifespan;
	float lifespanVary;                       // An individual particle's lifespan is added to by lifespanVary * random(-1, 1)
	track<float> emissionRate;
	float emissionRateVary;                   // This adds to the base emissionRate value the same way as lifespanVary. The random value is different every update.
	track<float> emissionAreaLength;        // For plane generators, this is the width of the plane in the x-axis.
	track<float> emissionAreaWidth;         // For plane generators, this is the width of the plane in the y-axis.
	track<float> zSource;                   // When greater than 0, the initial velocity of the particle is (particle.position - C3Vector(0, 0, zSource)).Normalize()
	fake_block<common_types::vector3> colorTrack;             // Most likely they all have 3 timestamps for {start, middle, end}.
	fake_block<std::uint16_t> alphaTrack;
	fake_block<common_types::vector2> scaleTrack;
	common_types::vector2 scaleVary;                      // A percentage amount to randomly vary the scale of each particle
	fake_block<std::uint16_t> headCellTrack;             // Some kind of intensity values seen: 0,16,17,32 (if set to different it will have high intensity)
	fake_block<std::uint16_t> tailCellTrack;
	float tailLength;                         // TailCellTime?
	float TwinkleSpeed;                       // has something to do with the spread
	float TwinklePercent;                     // has something to do with the spread
	range twinkleScale;
	float BurstMultiplier;                    // ivelScale
	float drag;                               // For a non-zero values, instead of travelling linearly the particles seem to slow down sooner. Speed is multiplied by exp( -drag * t ).
	float baseSpin;                           // Initial rotation of the particle quad
	float baseSpinVary;
	float Spin;                               // Rotation of the particle quad per second
	float spinVary;

	common_types::aa_box tumble;
	common_types::vector3 WindVector;
	float WindTime;

	float followSpeed1;
	float followScale1;
	float followSpeed2;
	float followScale2;
	iterator<common_types::vector3> splinePoints;                                  //Set only for spline praticle emitter. Contains array of points for spline
	track<std::uint8_t> enabledIn;                 // (boolean) Appears to be used sparely now, probably there's a flag that links particles to animation sets where they are enabled.
};

struct header
{
	std::uint32_t magic;
	version vers;
	iterator<char> name;
	md20_flags flags;

	iterator<loop> loops;
	iterator<sequence> sequences;
	iterator<std::uint16_t> sequences_lookups;
	iterator<compbone> bones;
	iterator<std::int16_t> key_bone_lookups;
	iterator<vertex> vertices;
	std::uint32_t num_skin_profiles;
	iterator<color> colors;
	iterator<texture> textures;
	iterator<texture_weight> texture_weights;
	iterator<texture_transform> texture_transforms;
	iterator<std::uint16_t> replacable_texture_lookup;
	iterator<material> materials;

	iterator<std::uint16_t> bone_lookup_table;
	iterator<std::uint16_t> texture_lookup_table;
	iterator<std::uint16_t> tex_unit_lookup_table;
	iterator<std::uint16_t> transparency_lookup_table;
	iterator<std::uint16_t> texture_transforms_lookup_table;

	common_types::aa_box bounding_box;
	float bounding_sphere_radius;
	common_types::aa_box collision_box;
	float collision_sphere_radius;
	iterator<std::uint16_t> collision_triangles;
	iterator<common_types::vector3> collision_vertices;
	iterator<common_types::vector3> collision_normals;
	iterator<attachment> attachments;

	iterator<std::uint16_t> attachment_lookup_table;
	iterator<event> events;
	iterator<light> lights;
	iterator<camera> cameras;
	iterator<std::uint16_t> camera_lookup_table;
	iterator<ribbon> ribbon_emitters;
	iterator<particle> particle_emitters;
};
}

struct md20_magic_tag_t{}md20_magic_tag;

class md21
{
	span<std::uint8_t> m;
	md20::header *hd;
	bool md20_tg=false;
public:
	md21(const span<std::uint8_t> &s):m(s)
	{
		using namespace std::string_literals;
		parser p(m.begin(),m.end());
		hd = p.at<md20::header>();
		if(hd->magic!=0x3032444D)
		{
			union
			{
				std::uint32_t u;
				std::array<char,4> a;
			}mg{hd->magic};
			throw std::runtime_error("not a md20 block : "s+std::to_string(mg.u)+'\t'+std::string(mg.a.begin(),mg.a.end()));
		}
	}
	md21(md20_magic_tag_t,const span<std::uint8_t> &s):m(s.begin()-4,s.end()),md20_tg(true)
	{
		using namespace std::string_literals;
		parser p(m.begin(),m.end());
		hd = p.at<md20::header>();
		if(hd->magic!=0x3032444D)
		{
			union
			{
				std::uint32_t u;
				std::array<char,4> a;
			}mg{hd->magic};
			throw std::runtime_error("not a md20 block : "s+std::to_string(mg.u)+'\t'+std::string(mg.a.begin(),mg.a.end()));
		}
	}
	
	decltype(auto) head() const
	{
		return *hd;
	}
	
	auto data() const
	{
		return m;
	}
	
	auto size() const
	{
		return m.size();
	}
	
	template<typename T>
	auto operator[](const md20::iterator<T> &i)
	{
		return span<T>(m.begin()+i.offset_elements,i.number);
	}
	
	template<typename T>
	auto operator[](const md20::iterator<T> &i) const
	{
		return span<const T>(m.cbegin()+i.offset_elements,i.number);
	}
	
	template<typename T>
	auto at(const md20::iterator<T> &i)
	{
		if(i.offset_elements+i.number*sizeof(T)<size())
			return operator[](i);
		throw std::out_of_range("md20 iterator out of range");
	}
	
	template<typename T>
	auto at(const md20::iterator<T> &i) const
	{
		if(i.offset_elements+i.number*sizeof(T)<size())
			return operator[](i);
		throw std::out_of_range("md20 iterator out of range");
	}
	
	bool is_md20() const
	{
		return md20_tg;
	}
	
	void set_attachment_position(std::size_t pos,const common_types::vector3& v)
	{
		decltype(auto) bones(at(hd->bones));
		decltype(auto) attachments(at(hd->attachments));
		std::size_t i(0);
		for(;i!=attachments.size();++i)
			if(attachments[i].id==pos)
				break;
		if(i==attachments.size())
			throw std::runtime_error("no this attachment in the model file");
		bones.at(attachments.at(i).bone).pivot=attachments.at(i).position=v;
	}
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const md21& v)
{
	if(v.is_md20())
		os<<"MD20\t";
	else
		os<<"MD21\t";
	os<<v.size()<<" bytes\t"<<v.head().vers<<'\t';
	auto s(v.at(v.head().name));
	if(s.back())
		os.rdbuf()->sputn(s.data(),s.size());
	else
		os<<s.data();
	if(v.head().flags.flag_unk_0x200000)
		os<<"\t(24500 upgraded model format)";
	return (os);
}	
}