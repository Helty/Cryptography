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
	static std::string Hashed(std::string const&);

private:
	static void Compression(ByteArray&, ByteArray const&);

	static std::vector<ByteArray> KeyGeneration(ByteArray const&, ByteArray const&);
	static ByteArray EncryptionTransformation(ByteArray const&, ByteMatrix const&);
	static void ShuffleTransformation(ByteArray&, ByteArray&, ByteArray const&);

	static void W(ByteArray&, size_t);
	static ByteArray E(ByteArray const&, ByteArray const&);
	static ByteArray P(ByteArray const&);
	static size_t fi(size_t const&);
	static ByteArray A(ByteArray const&);

	static void FeistelCipher(ByteArray&, ByteArray&, ByteMatrix const&);
	static void RoundOfFeistelCipher(ByteArray&, ByteArray&, ByteArray const&);
	static void SubstitutionTable(ByteArray&);

	static std::string ByteArrayToHash(ByteArray const&);
	static ByteArray MessageToByteArray(std::string const&);
	static ByteArray MessageBitSizeToByteArray(uint256_t const&);
	static void SplitByteArray32ToByteMatrix8Inv(ByteArray const&, ByteMatrix&);
};

