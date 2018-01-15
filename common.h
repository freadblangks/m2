#pramga once

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

}