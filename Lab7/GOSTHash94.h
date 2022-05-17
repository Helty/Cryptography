#pragma once
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::multiprecision;
using ByteArray = std::vector<uint8_t>;

class CGOSTHash94
{
public:
	static std::string Hashed(std::string const& message);

private:
	static void Compression(ByteArray& hash, ByteArray const& m);
};

