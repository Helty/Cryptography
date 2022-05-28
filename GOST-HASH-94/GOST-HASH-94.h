#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <bitset>

#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using ByteArray = std::vector<uint8_t>;
using ByteMatrix = std::vector<ByteArray>;

class CGOSTHash94
{
public:
	static std::string Hashed(std::string const& message);

private:
	static void Compression(ByteArray& hash, ByteArray const& m);

	static void TransformationW(ByteArray& Y, size_t counter);
	static ByteArray TransformationE(ByteArray const& h, ByteArray const& key);
	static size_t fi(size_t const& arg);
	static ByteArray TransformationP(ByteArray const& Y);
	static ByteArray TransformationA(ByteArray const& Y);

	static void ShuffleTransformation(ByteArray& hash, ByteArray& S, ByteArray const& M);
	static void SubstitutionTable(ByteArray& blocks4bytes);
	static void RoundOfFeistelCipher(ByteArray& A0, ByteArray& B0, ByteArray const& keys32);
	static void FeistelCipher(ByteArray& A0, ByteArray& B0, std::vector<ByteArray> const& keys32);
	static ByteArray EncryptionTransformation(ByteArray const& hash, std::vector<ByteArray> const& keys);
	static std::vector<ByteArray> KeyGeneration(ByteArray const& hash, ByteArray const& m);

	static ByteArray StringToByteArray(std::string const& str);
	static ByteArray Split256ToByteArray(uint256_t const& dec);
	static void Split256To32Inv(ByteArray const& from, std::vector<ByteArray>& to);
};

