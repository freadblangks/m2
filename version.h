#pragma once

#include<cstdint>

namespace m2
{
struct version
{
	std::uint32_t value;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& os,const version& v)
{
	switch(v.value)
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
			if(v.value<256)
				return os<<"Classic ("<<v.value<<')';
			else if(260<v.value&&v.value<264)
				return os<<"The Burning Crusade ("<<v.value<<')';
			else if(264<v.value&&v.value<272)
				return os<<"Cataclysm ("<<v.value<<')';
			else
				return os<<"Unknown Expansion ("<<v.value<<')';
	}
}
}