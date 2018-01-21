#pragma once
#include<string>
#include<vector>
#include"block.h"
#include"md20.h"
#include"unknown.h"

namespace m2
{
	
struct m2
{
	std::vector<block> blocks;
	m2(const std::string &s)
	{
		for(auto it(s.data()),en(s.data()+s.size());it<en;)
		{
			auto pmg(reinterpret_cast<const std::uint32_t*>(it));
			auto mg(*pmg);
			auto size(pmg[1]);
			const std::string s(it+8,it+8+size);
			switch(mg)
			{
				case 0x3132444D:
					blocks.emplace_back(std::in_place_type<md20>,s);
				break;
				default:
					blocks.emplace_back(std::in_place_type<unknown>,mg,s);
			}
			it+=8+size;
		}
	}
	std::string serialize() const
	{
		std::string r;
		for(const auto &ele : blocks)
			r.append(ele.serialize());
		return r;
	}
	template<typename T>
	decltype(auto) get()
	{
		for(auto &ele : blocks)
			if(ele.type()==typeid(T))
				return ele.get<T>();
		throw std::logic_error("md file does not have this type");
	}

	template<typename T>
	decltype(auto) get() const
	{
		for(auto &ele : blocks)
			if(ele.type()==typeid(T))
				return ele.get<T>();
		throw std::logic_error("md file does not have this type");
	}
};

template<typename ostrm>
ostrm& operator<<(ostrm& os,const m2& u)
{
	for(std::size_t i(0);i!=u.blocks.size();++i)
	{
		if(i)
			os.put('\n');
		os<<u.blocks[i];
	}
	return os;
}

};