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
	std::vector<event> events;
	std::vector<light> lights;
	std::vector<camera> cameras;
	std::vector<std::uint16_t> camera_lookup_table;
	std::vector<ribbon> ribbons;
	std::vector<std::uint16_t> texture_combiner_combos;
//	std::vector<particle> particles;
	md20(const std::string &s)
	{
		if(s.front()!='M'||s[1]!='D'||s[2]!='2'||s[3]!='0')
			throw std::runtime_error("not MD20 which is ("s+s.substr(0,4)+")"s);
		decltype(auto) header(*reinterpret_cast<const dh::dheader*>(s.data()+4));
		auto m([&s](auto &vec,const auto& off)
		{
			auto b(reinterpret_cast<typename std::remove_reference_t<decltype(vec)>::const_pointer>(s.data()+off.offset_elements));
			auto e(b+off.number);
			vec.assign(b,e);
		});
		auto pt_base([&s,&m](auto &trk,const auto& t)
		{
			trk.t=t.t;
			{
			auto b(reinterpret_cast<const offset<void>*>(s.data()+t.timestamps.offset_elements));
			for(std::size_t i(0);i!=t.timestamps.number;++i)
			{
				trk.timestamps.emplace_back();
				m(trk.timestamps.back(),b[i]);
			}
			}
		});
		auto pt([&s,&m,&pt_base](auto &trk,const auto& t)
		{
			pt_base(trk,t);
			{
			auto b(reinterpret_cast<const offset<void>*>(s.data()+t.values.offset_elements));
			for(std::size_t i(0);i!=t.values.number;++i)
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
		{
		auto b(reinterpret_cast<const dh::event*>(s.data()+header.events.offset_elements));
		for(std::size_t i(0);i!=header.events.number;++i)
		{
			decltype(auto) ele(b[i]);
			events.emplace_back();
			auto &back(events.back());
			back.t=ele.t;
			pt_base(back.enabled,ele.enabled);
		}
		}
		{
		auto b(reinterpret_cast<const dh::light*>(s.data()+header.lights.offset_elements));
		for(std::size_t i(0);i!=header.lights.number;++i)
		{
			decltype(auto) ele(b[i]);
			lights.emplace_back();
			auto &back(lights.back());
			back.t=ele.t;
			pt(back.ambient_color,ele.ambient_color);
			pt(back.ambient_intensity,ele.ambient_intensity);
			pt(back.diffuse_color,ele.diffuse_color);
			pt(back.diffuse_intensity,ele.diffuse_intensity);
			pt(back.attenuation_start,ele.attenuation_start);
			pt(back.attenuation_end,ele.attenuation_end);
			pt(back.visibility,ele.visibility);
		}
		}
		{
		auto b(reinterpret_cast<const dh::camera*>(s.data()+header.cameras.offset_elements));
		for(std::size_t i(0);i!=header.cameras.number;++i)
		{
			decltype(auto) ele(b[i]);
			cameras.emplace_back();
			auto &back(cameras.back());
			back.t=ele.t;
			pt(back.positions,ele.positions);
			back.position_base=ele.position_base;
			pt(back.target_position,ele.target_position);
			back.target_position_base=ele.target_position_base;
			pt(back.roll,ele.roll);
			pt(back.fov,ele.fov);
		}
		}
		{
		auto b(reinterpret_cast<const dh::ribbon*>(s.data()+header.ribbon_emitters.offset_elements));
		for(std::size_t i(0);i!=header.ribbon_emitters.number;++i)
		{
			decltype(auto) ele(b[i]);
			ribbons.emplace_back();
			auto &back(ribbons.back());
			back.t=ele.t;
			m(back.texture_indices,ele.texture_indices);
			m(back.material_indices,ele.material_indices);
			pt(back.color,ele.color);
			pt(back.alpha,ele.alpha);
			pt(back.height_above,ele.height_above);
			pt(back.height_below,ele.height_below);
			back.edge_per_second=ele.edge_per_second;
			back.edge_life_time=ele.edge_life_time;
			back.gravity=ele.gravity;
			back.texture_rows=ele.texture_rows;
			back.texture_cols=ele.texture_cols;
			pt(back.texture_slot,ele.texture_slot);
			pt(back.visibility,ele.visibility);
			back.priority_plane=ele.priority_plane;
			back.padding=ele.padding;
		}
		}
		m(camera_lookup_table,header.camera_lookup_table);
		if(flags.flag_use_texture_combiner_combos)
			m(texture_combiner_combos,header.texture_combiner_combos);
	}
	auto serialize_md20() const
	{
		std::string s("MD20"s);
		dh::dheader header;
		s.resize(s.size()+sizeof(header),0);
		auto m([&s](const auto &vec,auto& off)
		{
			static_assert(std::is_same_v<typename std::remove_reference_t<std::remove_const_t<decltype(vec)>>::value_type,
						typename std::remove_reference_t<decltype(off)>::value_type>);
			off.offset_elements=s.size();
			off.number=vec.size();
			s.append(reinterpret_cast<const char*>(vec.data()),reinterpret_cast<const char*>(vec.data()+vec.size()));
		});
		auto ua([&s](const auto &vec,auto& off)
		{
			off.offset_elements=s.size();
			off.number=vec.size();
			auto p(s.size());
			s.append(sizeof(typename std::remove_reference_t<decltype(off)>::value_type)*vec.size(),0);
			return reinterpret_cast<typename std::remove_reference_t<decltype(off)>::pointer>(s.data()+p);
		});
		auto pt_base([&m,&ua](const auto &trk,auto& t)
		{
			t.t=trk.t;
			auto p(ua(trk.timestamps,t.timestamps));
			for(std::size_t i(0);i!=trk.timestamps.size();++i)
				m(trk.timestamps[i],p[i]);
		});
		auto pt([&m,&ua,&pt_base](const auto &trk,auto& t)
		{
			pt_base(trk,t);
			auto p(ua(trk.values,t.values));
			for(std::size_t i(0);i!=trk.values.size();++i)
				m(trk.values[i],p[i]);
		});
		header.ver=ver;
		m(name,header.name);
		header.flags=flags;
		m(loops,header.loops);
		m(sequences,header.sequences);
		
		
		m(sequences_lookups,header.sequences_lookups);
		{
		auto b(ua(bones,header.bones));
		for(std::size_t i(0);i!=bones.size();++i)
		{
			auto &ele(b[i]);
			const auto &back(bones[i]);
			ele.c=back.c;
			ele.pivot=back.pivot;
			pt(back.translation,ele.translation);
			pt(back.rotation,ele.rotation);
			pt(back.scale,ele.scale);
		}
		}
		m(key_bone_lookups,header.key_bone_lookups);
		m(vertices,header.vertices);
		header.num_skin_profiles=num_skin_profiles;
		{
		auto b(ua(colors,header.colors));

		for(std::size_t i(0);i!=colors.size();++i)
		{
			auto &ele(b[i]);
			const auto &back(colors[i]);
			pt(back.c,ele.c);
			pt(back.a,ele.a);
		}
		}
		{
		auto b(ua(textures,header.textures));
		for(std::size_t i(0);i!=textures.size();++i)
		{
			b[i].t=textures[i].t;
			m(textures[i].filename,b[i].filename);
		}
		}
		{
		auto b(ua(texture_weights,header.texture_weights));
		for(std::size_t i(0);i!=texture_weights.size();++i)
			pt(texture_weights[i],b[i]);
		}
		{
/*		auto b(reinterpret_cast<const dh::texture_transform*>(s.data()+header.texture_transforms.offset_elements));
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
		{
		auto b(reinterpret_cast<const dh::event*>(s.data()+header.events.offset_elements));
		for(std::size_t i(0);i!=header.events.number;++i)
		{
			decltype(auto) ele(b[i]);
			events.emplace_back();
			auto &back(events.back());
			back.t=ele.t;
			pt_base(back.enabled,ele.enabled);
		}
		}
		{
		auto b(reinterpret_cast<const dh::light*>(s.data()+header.lights.offset_elements));
		for(std::size_t i(0);i!=header.lights.number;++i)
		{
			decltype(auto) ele(b[i]);
			lights.emplace_back();
			auto &back(lights.back());
			back.t=ele.t;
			pt(back.ambient_color,ele.ambient_color);
			pt(back.ambient_intensity,ele.ambient_intensity);
			pt(back.diffuse_color,ele.diffuse_color);
			pt(back.diffuse_intensity,ele.diffuse_intensity);
			pt(back.attenuation_start,ele.attenuation_start);
			pt(back.attenuation_end,ele.attenuation_end);
			pt(back.visibility,ele.visibility);
		}
		}
		{
		auto b(reinterpret_cast<const dh::camera*>(s.data()+header.cameras.offset_elements));
		for(std::size_t i(0);i!=header.cameras.number;++i)
		{
			decltype(auto) ele(b[i]);
			cameras.emplace_back();
			auto &back(cameras.back());
			back.t=ele.t;
			pt(back.positions,ele.positions);
			back.position_base=ele.position_base;
			pt(back.target_position,ele.target_position);
			back.target_position_base=ele.target_position_base;
			pt(back.roll,ele.roll);
			pt(back.fov,ele.fov);
		}
		}
		{
		auto b(reinterpret_cast<const dh::ribbon*>(s.data()+header.ribbon_emitters.offset_elements));
		for(std::size_t i(0);i!=header.ribbon_emitters.number;++i)
		{
			decltype(auto) ele(b[i]);
			ribbons.emplace_back();
			auto &back(ribbons.back());
			back.t=ele.t;
			m(back.texture_indices,ele.texture_indices);
			m(back.material_indices,ele.material_indices);
			pt(back.color,ele.color);
			pt(back.alpha,ele.alpha);
			pt(back.height_above,ele.height_above);
			pt(back.height_below,ele.height_below);
			back.edge_per_second=ele.edge_per_second;
			back.edge_life_time=ele.edge_life_time;
			back.gravity=ele.gravity;
			back.texture_rows=ele.texture_rows;
			back.texture_cols=ele.texture_cols;
			pt(back.texture_slot,ele.texture_slot);
			pt(back.visibility,ele.visibility);
			back.priority_plane=ele.priority_plane;
			back.padding=ele.padding;
		}*/
		}
		m(camera_lookup_table,header.camera_lookup_table);
		if(flags.flag_use_texture_combiner_combos)
			m(texture_combiner_combos,header.texture_combiner_combos);
		
		*reinterpret_cast<dh::dheader*>(s.data()+4)=header;
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