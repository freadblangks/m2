#pragma once

#include<string>
#include<cstdint>
#include<array>
#include"magic.h"

namespace m2
{

struct unknown
{
	magic m;
	std::string unks;
	template<typename ...Args>
	unknown(std::uint32_t mgc,Args&& ...args):m{mgc},unks(std::forward<Args>(args)...){}
	std::string serialize() const
	{
		std::string s(m.a.cbegin(),m.a.cend());
		magic v{static_cast<std::uint32_t>(unks.size())};
		s.append(v.a.cbegin(),v.a.cend());
		s.append(unks);
		return s;
	}
};

template<typename ostrm>
ostrm& operator<<(ostrm& os,const unknown& u)
{
	os.rdbuf()->sputn(u.m.a.data(),u.m.a.size());
	return os<<"\tsize:"<<u.unks.size();
}

}