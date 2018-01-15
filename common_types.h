#pramga once

namespace m2
{
namespace common_types
{
struct vector2
{
	float x;
	float y;
};
struct vector3
{
	std::uint32_t x,y,z;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm &out,const vector3 &v3)
{
	struct b
	{
		float x,y,z;
	};
	union
	{
		vector3 v1;
		b v;
	}u{v3};
	return out<<u.v.x<<","<<u.v.y<<","<<u.v.z<<" {"<<v3.x<<","<<v3.y<<","<<v3.z<<"}";
}

struct vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct aa_box
{
	vector3 min;
	vector3 max;
};
}
}