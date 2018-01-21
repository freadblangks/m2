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