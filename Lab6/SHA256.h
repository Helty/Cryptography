#pragma once
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

class SHA256
{
public:

	static std::string Hashed(std::string const& message);

private:

	static std::vector<bool> ProcessingPreliminary(std::string const& message);
	static void Processing512(uint512_t const& value, std::vector<uint32_t>& initialH);
	static void Split512To32(uint512_t const& block512, std::vector<uint32_t>& blocks32);
	static uint32_t RightRotate(uint32_t block32, uint8_t bias);
	static size_t GenerateK(uint64_t L);
	static void AddMessageTo(std::vector<bool>& m, std::string const& message, size_t& lengthBinaryString);
	static std::string StringToStringBitSequence(std::string const& message);
	static uint512_t VectorBoolToDecimal(std::vector<bool>& sub512);
	static std::string DecimalToStringBitSequence(uint64_t L);
};

