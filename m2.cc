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
//	std::ofstream fout(argv[2],std::ofstream::binary);
	auto serialize(m.serialize());
//	fout.rdbuf()->sputn(serialize.data(),serialize.size());

}
catch(const std::exception& ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}
}