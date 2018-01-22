#pragma once

#include<string>
#include"magic.h"
#include"version.h"
#include"special.h"
#include"dheader.h"
#include<stdexcept>
#include<iterator>
#include"iterator.h"

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
	std::vector<particle> particles;
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
		auto fake_pt([&s,&m](auto &trk,const auto& t)
		{
			{
			auto b(reinterpret_cast<const offset<void>*>(s.data()+t.timestamps.offset_elements));
			for(std::size_t i(0);i!=t.timestamps.number;++i)
			{
				trk.timestamps.emplace_back();
				m(trk.timestamps.back(),b[i]);
			}
			}
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
		{
		auto b(reinterpret_cast<const dh::particle*>(s.data()+header.particle_emitters.offset_elements));
		for(std::size_t i(0);i!=header.particle_emitters.number;++i)
		{
			decltype(auto) ele(b[i]);
			particles.emplace_back();
			auto &back(particles.back());
			back.t=ele.t;
			m(back.geometry_model_filename,ele.geometry_model_filename);
			m(back.recursion_model_filename,ele.recursion_model_filename);
			back.m=ele.m;
			pt(back.emission_speed,ele.emission_speed);
			pt(back.speed_variation,ele.speed_variation);
			pt(back.vertical_range,ele.vertical_range);
			pt(back.horizontal_range,ele.horizontal_range);
			pt(back.gravity,ele.gravity);
			pt(back.lifespan,ele.lifespan);
			back.lifespan_vary=ele.lifespan_vary;
			pt(back.emission_rate,ele.emission_rate);
			back.emission_rate_vary=ele.emission_rate_vary;
			pt(back.emission_area_length,ele.emission_area_length);
			pt(back.emission_area_width,ele.emission_area_width);
			pt(back.z_source,ele.z_source);
			fake_pt(back.color_track,ele.color_track);
			fake_pt(back.alpha_track,ele.alpha_track);
			fake_pt(back.scale_track,ele.scale_track);
			back.scale_vary=ele.scale_vary;
			fake_pt(back.head_cell_track,ele.head_cell_track);
			fake_pt(back.tail_cell_track,ele.tail_cell_track);
			back.pm=ele.pm;
			pt(back.spline_points,ele.spline_points);
			pt(back.enabled_in,ele.enabled_in);
			back.multi_texture_params=ele.multi_texture_params;
		}
		}
		if(flags.flag_use_texture_combiner_combos)
			m(texture_combiner_combos,header.texture_combiner_combos);
	}
	auto serialize_md20() const
	{
		auto s("MD20"s);
		dh::dheader header{};
		s.resize(s.size()+sizeof(header),0);
		auto m([&s](const auto &vec,auto& off)
		{
			static_assert(std::is_same_v<typename std::remove_reference_t<std::remove_const_t<decltype(vec)>>::value_type,
						typename std::remove_reference_t<decltype(off)>::value_type>);
			off.offset_elements=s.size();
			off.number=vec.size();
			m2_iterator<typename std::remove_reference_t<decltype(off)>::value_type> i(s,s.size());
			s.append(reinterpret_cast<const char*>(vec.data()),reinterpret_cast<const char*>(vec.data()+vec.size()));
			return i;
		});
		auto ua([&s](const auto &vec,auto& off)
		{
			off.offset_elements=s.size();
			off.number=vec.size();
			m2_iterator<typename std::remove_reference_t<decltype(off)>::value_type> i(s,s.size());
			s.append(sizeof(typename std::remove_reference_t<decltype(off)>::value_type)*vec.size(),0);
			return i;
		});
		auto pt_base([&m,&ua](const auto &trk,auto t)
		{
			t.t=trk.t;
			auto p(ua(trk.timestamps,t.timestamps));
			for(std::size_t i(0);i!=trk.timestamps.size();++i)
				m(trk.timestamps[i],p[i]);
			return t;
		});
		auto pt([&m,&ua](const auto &trk,auto t)
		{
			{
			t.t=trk.t;
			auto p(ua(trk.timestamps,t.timestamps));
			for(std::size_t i(0);i!=trk.timestamps.size();++i)
				m(trk.timestamps[i],p[i]);
			}
			{
			auto p(ua(trk.values,t.values));
			for(std::size_t i(0);i!=trk.values.size();++i)
				m(trk.values[i],p[i]);
			}
			return t;
		});
		auto fake_pt([&m,&ua](auto &trk,auto t)
		{
			{
			auto p(ua(trk.timestamps,t.timestamps));
			for(std::size_t i(0);i!=trk.timestamps.size();++i)
				m(trk.timestamps[i],p[i]);
			}
			{
			auto p(ua(trk.values,t.values));
			for(std::size_t i(0);i!=trk.values.size();++i)
				m(trk.values[i],p[i]);
			}
			return t;
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
			const auto &back(bones[i]);
			b[i].c=back.c;
			b[i].pivot=back.pivot;
			b[i].translation=pt(back.translation,b[i].translation);
			b[i].rotation=pt(back.rotation,b[i].rotation);
			b[i].scale=pt(back.scale,b[i].scale);
		}
		}
		m(key_bone_lookups,header.key_bone_lookups);
		m(vertices,header.vertices);
		header.num_skin_profiles=num_skin_profiles;
		{
		auto b(ua(colors,header.colors));

		for(std::size_t i(0);i!=colors.size();++i)
		{
			const auto &back(colors[i]);
			b[i].c=pt(back.c,b[i].c);
			b[i].a=pt(back.a,b[i].a);
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
			b[i]=pt(texture_weights[i],b[i]);
		}
		{
		auto b(ua(texture_transforms,header.texture_transforms));
		for(std::size_t i(0);i!=texture_transforms.size();++i)
		{
			auto &back(texture_transforms[i]);
			b[i].translation=pt(back.translation,b[i].translation);
			b[i].rotation=pt(back.rotation,b[i].rotation);
			b[i].scaling=pt(back.scaling,b[i].scaling);
		}
		}
		m(replacable_texture_lookup,header.replacable_texture_lookup);
		m(materials,header.materials);
		m(bone_lookup_table,header.bone_lookup_table);
		m(texture_lookup_table,header.texture_lookup_table);
		m(tex_unit_lookup_table,header.tex_unit_lookup_table);
		m(transparency_lookup_table,header.transparency_lookup_table);
		m(texture_transforms_lookup_table,header.texture_transforms_lookup_table);
		header.bounding_box=bounding_box;
		header.bounding_sphere_radius=bounding_sphere_radius;
		header.collision_box=collision_box;
		header.collision_sphere_radius=collision_sphere_radius;
		m(collision_triangles,header.collision_triangles);
		m(collision_vertices,header.collision_vertices);
		m(collision_normals,header.collision_normals);
		{
		auto b(ua(attachments,header.attachments));
		for(std::size_t i(0);i!=attachments.size();++i)
		{
			auto &back(attachments[i]);
			b[i].t=back.t;
			b[i].animate_attached=pt(back.animate_attached,b[i].animate_attached);
		}
		}
		m(attachment_lookup_table,header.attachment_lookup_table);
		{
		auto b(ua(events,header.events));
		for(std::size_t i(0);i!=events.size();++i)
		{
			auto &back(events[i]);
			b[i].t=back.t;
			b[i].enabled=pt_base(back.enabled,b[i].enabled);
		}
		}
		{
		auto b(ua(lights,header.lights));
		for(std::size_t i(0);i!=lights.size();++i)
		{
			auto &back(lights[i]);
			b[i].t=back.t;
			b[i].ambient_color=pt(back.ambient_color,b[i].ambient_color);
			b[i].ambient_intensity=pt(back.ambient_intensity,b[i].ambient_intensity);
			b[i].diffuse_color=pt(back.diffuse_color,b[i].diffuse_color);
			b[i].diffuse_intensity=pt(back.diffuse_intensity,b[i].diffuse_intensity);
			b[i].attenuation_start=pt(back.attenuation_start,b[i].attenuation_start);
			b[i].attenuation_end=pt(back.attenuation_end,b[i].attenuation_end);
			b[i].visibility=pt(back.visibility,b[i].visibility);
		}
		}
		{
		auto b(ua(cameras,header.cameras));
		for(std::size_t i(0);i!=cameras.size();++i)
		{
			auto &back(cameras[i]);
			b[i].t=back.t;
			b[i].positions=pt(back.positions,b[i].positions);
			b[i].position_base=back.position_base;
			b[i].target_position=pt(back.target_position,b[i].target_position);
			b[i].target_position_base=b[i].target_position_base;
			b[i].roll=pt(back.roll,b[i].roll);
			b[i].fov=pt(back.fov,b[i].fov);
		}
		}
		{
		auto b(ua(ribbons,header.ribbon_emitters));
		for(std::size_t i(0);i!=ribbons.size();++i)
		{
			auto &back(ribbons[i]);
			b[i].t=back.t;
			m(back.texture_indices,b[i].texture_indices);
			m(back.material_indices,b[i].material_indices);
			b[i].color=pt(back.color,b[i].color);
			b[i].alpha=pt(back.alpha,b[i].alpha);
			b[i].height_above=pt(back.height_above,b[i].height_above);
			b[i].height_below=pt(back.height_below,b[i].height_below);
			b[i].edge_per_second=back.edge_per_second;
			b[i].edge_life_time=back.edge_life_time;
			b[i].gravity=back.gravity;
			b[i].texture_rows=back.texture_rows;
			b[i].texture_cols=back.texture_cols;
			b[i].texture_slot=pt(back.texture_slot,b[i].texture_slot);
			b[i].visibility=pt(back.visibility,b[i].visibility);
			b[i].priority_plane=back.priority_plane;
			b[i].padding=back.padding;
		}
		}
		m(camera_lookup_table,header.camera_lookup_table);
		if(flags.flag_use_texture_combiner_combos)
			m(texture_combiner_combos,header.texture_combiner_combos);
		*reinterpret_cast<dh::dheader*>(s.data()+4)=header;
		return s;
	}
	auto serialize() const
	{
		auto s("MD21"s);
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