#pragma once
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::multiprecision;

class CGOSTHash94
{
public:

	static std::string Hashed(std::string const& message);

private:
	static void Compression(uint256_t& hash, uint256_t const& m);
};

