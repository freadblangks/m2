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
	std::cout<<m<<'\n';
	decltype(auto) md20(m.get<m2::md20>());
	std::cout<<md20.textures.size()<<'\n';
	for(const auto &ele : md20.textures)
	{
		std::cout<<ele<<'\n';
	}
}
catch(const std::exception& ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}
}