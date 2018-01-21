#pragma once

#include<string>

namespace m2
{

class block
{
	struct base
	{
		virtual ~base() = default;
		virtual base* clone() const = 0;
		virtual std::string serialize() const = 0;
	}*u;
	template<typename T>
	struct derv:base
	{
		T t;
		template<typename ...Args>
		derv(Args&& ...args):t(std::forward<Args>(args)...){}
		base* clone() const
		{
			return new derv(*this);
		}
		std::string serialize() const
		{
			return t.serialize();
		}
	};
public:
	template<typename T,typename ...Args>
	block(std::in_place_type_t<T>,Args&& ...args):u(new derv<T>(std::forward<Args>(args)...)){}
	block(const block& b):u(b.u->clone()){}
	block& operator=(const block& b)
	{
		auto ptr(b.u->clone());
		delete u;
		u = ptr;
		return *this;
	}
	block(block &&b) noexcept:u(b.u)
	{
		b.u=nullptr;
	};
	
	block& operator=(block&& b) noexcept
	{
		if(&b!=this)
		{
			delete u;
			u=b.u;
			b.u=nullptr;
		}
		return *this;
	}
	~block()
	{
		delete u;
	}
	
	std::string serialize() const
	{
		return u->serialize();
	}
};

}