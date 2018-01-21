#pragma once

#include<string>
#include"magic.h"
#include"version.h"
#include"special.h"
#include"dheader.h"
#include<stdexcept>

namespace m2
{
using namespace std::string_literals;
struct md20
{
	version ver;
	std::string name;
	md20_flags flags;
	std::vector<loop> loops;
	std::vector<sequence> sequences;
	std::vector<std::uint16_t> sequences_lookups;
	std::vector<compbone> bones;
	std::vector<std::uint16_t> key_bone_lookups;
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
	std::vector<common_types::vector3> collision_vertices,collision_normals;
	std::vector<attachment> attachments;
	std::vector<std::uint16_t> attachment_lookup_table;
	md20(const std::string &s)
	{
		if(s.front()!='M'||s[1]!='D'||s[2]!='2'||s[3]!='0')
			throw std::runtime_error("not MD20 which is ("s+s.substr(0,4)+")"s);
		decltype(auto) header(*reinterpret_cast<const dheader*>(s.data()+4));
		auto m([&s](auto &vec,const offset& off)
		{
			auto b(reinterpret_cast<typename std::remove_reference_t<decltype(vec)>::const_pointer>(s.data()+off.offset_elements));
			auto e(b+off.number);
			vec.assign(b,e);
		});
		auto pt([&s,&m](auto &trk,const dh::track& t)
		{
			trk.t=t.t;
			{
			auto b(reinterpret_cast<const offset*>(s.data()+t.timestamps.offset_elements));
			for(std::size_t i(0);i!=t.timestamps.number;++i)
			{
				trk.timestamps.emplace_back();
				m(trk.timestamps.back(),b[i]);
			}
			}
			{
			auto b(reinterpret_cast<const offset*>(s.data()+t.timestamps.offset_elements));
			for(std::size_t i(0);i!=t.timestamps.number;++i)
			{
				trk.values.emplace_back();
				m(trk.values.back(),b[i]);
			}
			}
		});
		ver=header.ver;
		m(name,header.name);
		flags=header.flags;
		m(loops,header.loops);
		m(sequences,header.sequences);
		m(sequences_lookups,header.sequences_lookups);
		{
		auto b(reinterpret_cast<const dh::compbone*>(s.data()+header.bones.offset_elements));
		for(std::size_t i(0);i!=header.bones.number;++i)
		{
			decltype(auto) ele(b[i]);
			bones.emplace_back();
			auto &back(bones.back());
			back.c=ele.c;
			back.pivot=ele.pivot;
			pt(back.translation,ele.translation);
			pt(back.rotation,ele.rotation);
			pt(back.scale,ele.scale);
		}
		}
		m(key_bone_lookups,header.key_bone_lookups);
		m(vertices,header.vertices);
		num_skin_profiles=header.num_skin_profiles;
		{
		auto b(reinterpret_cast<const dh::color*>(s.data()+header.colors.offset_elements));
		for(std::size_t i(0);i!=header.colors.number;++i)
		{
			decltype(auto) ele(b[i]);
			colors.emplace_back();
			auto &back(colors.back());
			pt(back.c,ele.c);
			pt(back.a,ele.a);
		}
		}
		{
		auto b(reinterpret_cast<const dh::texture*>(s.data()+header.textures.offset_elements));
		for(std::size_t i(0);i!=header.textures.number;++i)
		{
			textures.emplace_back();
			textures.back().t=b[i].t;
			m(textures.back().filename,b[i].filename);
		}
		}
		{
		auto b(reinterpret_cast<const dh::texture_weight*>(s.data()+header.texture_weights.offset_elements));
		for(std::size_t i(0);i!=header.texture_weights.number;++i)
		{
			texture_weights.emplace_back();
			pt(texture_weights.back(),b[i]);
		}
		}
		{
		auto b(reinterpret_cast<const dh::texture_transform*>(s.data()+header.texture_transforms.offset_elements));
		for(std::size_t i(0);i!=header.texture_transforms.number;++i)
		{
			decltype(auto) ele(b[i]);
			texture_transforms.emplace_back();
			auto &back(texture_transforms.back());
			pt(back.translation,ele.translation);
			pt(back.rotation,ele.rotation);
			pt(back.scaling,ele.scaling);
		}
		}
		m(replacable_texture_lookup,header.replacable_texture_lookup);
		m(materials,header.materials);
		m(bone_lookup_table,header.bone_lookup_table);
		m(texture_lookup_table,header.texture_lookup_table);
		m(tex_unit_lookup_table,header.tex_unit_lookup_table);
		m(transparency_lookup_table,header.transparency_lookup_table);
		m(texture_transforms_lookup_table,header.texture_transforms_lookup_table);
		bounding_box=header.bounding_box;
		bounding_sphere_radius=header.bounding_sphere_radius;
		collision_box=header.collision_box;
		collision_sphere_radius=header.collision_sphere_radius;
		m(collision_triangles,header.collision_triangles);
		m(collision_vertices,header.collision_vertices);
		m(collision_normals,header.collision_normals);
		{
		auto b(reinterpret_cast<const dh::attachment*>(s.data()+header.attachments.offset_elements));
		for(std::size_t i(0);i!=header.attachments.number;++i)
		{
			decltype(auto) ele(b[i]);
			attachments.emplace_back();
			auto &back(attachments.back());
			back.t=ele.t;
			pt(back.animate_attached,ele.animate_attached);
		}
		}
		m(attachment_lookup_table,header.attachment_lookup_table);
	}
	auto serialize_md20() const
	{
		std::string s("MD20");

		return s;
	}
	auto serialize() const
	{
		std::string s("MD21");
		auto p(serialize_md20());	
		magic m{static_cast<std::uint32_t>(p.size())};		
		s.append(m.a.cbegin(),m.a.cend());
		s.append(p);
		return s;
	}
};

template<typename ostrm>
ostrm& operator<<(ostrm& os,const md20& u)
{
	os<<"MD20\t"<<u.name<<'\t'<<u.ver<<'\t'<<u.flags;
	return os;
}

}