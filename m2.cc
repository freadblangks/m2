#include"m2.h"
#include<iostream>
#include<fstream>
#include<string>
#include<iterator>

int main(int argc,char** argv)
{
try
{
	if(argc<2)
	{
		std::cerr<<"Usage : "<<*argv<<" <m2 filename path>\n";
		return 1;
	}
	std::ifstream fin(argv[1],std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string file((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	m2::m2 m(file);
	auto &md20(m.get<m2::md20>());
	for(const auto &ele : md20.particles)
	{
		std::cout<<ele.geometry_model_filename<<'\n';
	}
//	std::ofstream fout(argv[2],std::ofstream::binary);
	auto serialize(m.serialize());
	{
		m2::m2 m1(serialize);
		auto s1(m1.serialize());
//		std::cout<<file.size()<<' '<<serialize.size()<<' '<<s1.size()<<'\n';
		std::cout<<(s1==serialize)<<'\n';
	
		for(std::size_t i(0);i!=serialize.size()&&i!=s1.size();++i)
			if(serialize[i]!=s1[i])
				std::cout<<i<<' '<<static_cast<std::uint32_t>(serialize[i])<<' '<<static_cast<std::uint32_t>(s1[i])<<'\n';
	}
	
//	fout.rdbuf()->sputn(serialize.data(),serialize.size());

}
catch(const std::exception& ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}
}