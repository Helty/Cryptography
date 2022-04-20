#include "SHA256.h"
#include <bitset>
#include <optional>

namespace
{
	std::vector<uint32_t> hVector =
	{
		0x6A09E667,
		0xBB67AE85,
		0x3C6EF372,
		0xA54FF53A,
		0x510E527F,
		0x9B05688C,
		0x1F83D9AB,
		0x5BE0CD19
	};

	std::vector<uint32_t> kVector =
	{
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
		0x3956C25B,	0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
		0xD807AA98, 0x12835B01,	0x243185BE, 0x550C7DC3,
		0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7,	0xC19BF174,
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
		0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
		0x983E5152,	0xA831C66D, 0xB00327C8, 0xBF597FC7,
		0xC6E00BF3, 0xD5A79147,	0x06CA6351, 0x14292967,
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
		0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
		0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
		0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
		0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
	};
}

std::string SHA256::Hashed(std::string const& message)
{
	std::string hash;
	std::vector<bool> m = ProcessingPreliminary(message);
	std::vector<uint32_t> initialH(hVector.begin(), hVector.end()); //h0 h1 h2 h3 h4 h5 h6 h7

	for (size_t i = 0; i < m.size(); i += 512)
	{
		std::vector<bool> block512(m.begin() + i, m.begin() + i + 512);
		uint512_t value = VectorBoolToDecimal(block512);
		Processing512(value, initialH);
	}

	std::stringstream sstream;
	for (size_t i = 0; i < initialH.size(); i++) sstream << std::hex << initialH[i];
	hash.append(sstream.str());

	std::transform(hash.begin(), hash.end(), hash.begin(),
		[](unsigned char c) { return std::toupper(c); });

	return hash;
}

void SHA256::Processing512(uint512_t const& value, std::vector<uint32_t>& initialH)
{
	//step 1
	std::vector<uint32_t> blocks32;
	{
		Split512To32(value, blocks32);
	}

	//step 2
	{
		for (size_t i = 16; i < 64; ++i)
		{
			uint32_t s0 = RightRotate(blocks32[i - 15], 7) ^ RightRotate(blocks32[i - 15], 18) ^ (blocks32[i - 15] >> 3);
			uint32_t s1 = RightRotate(blocks32[i - 2], 17) ^ RightRotate(blocks32[i - 2], 19) ^ (blocks32[i - 2] >> 10);
			blocks32.push_back(blocks32[i - 16] + s0 + blocks32[i - 7] + s1);
		}
	}

	std::vector<uint32_t> initH(initialH);

	//step 3
	{
		for (size_t i = 0; i < 64; ++i)
		{
			uint32_t s0 = RightRotate(initH[0], 2) ^ RightRotate(initH[0], 13) ^ RightRotate(initH[0], 22);
			uint32_t Ma = (initH[0] & initH[1]) ^ (initH[0] & initH[2]) ^ (initH[1] & initH[2]);
			uint32_t t2 = s0 + Ma;
			uint32_t s1 = RightRotate(initH[4], 6) ^ RightRotate(initH[4], 11) ^ RightRotate(initH[4], 25);
			uint32_t Ch = (initH[4] & initH[5]) ^ ((~initH[4]) & initH[6]);
			uint32_t t1 = initH[7] + s1 + Ch + kVector[i] + blocks32[i];

			initH[7] = initH[6];
			initH[6] = initH[5];
			initH[5] = initH[4];
			initH[4] = (initH[3] + t1);
			initH[3] = initH[2];
			initH[2] = initH[1];
			initH[1] = initH[0];
			initH[0] = (t1 + t2);
		}
	}

	//step 4
	{
		for (size_t i = 0; i < 8; i++) initialH[i] += initH[i];
	}
}

uint32_t SHA256::RightRotate(uint32_t block32, uint8_t bias)
{
	return (block32 >> bias) | (block32 << (32 - bias));
}

void SHA256::Split512To32(uint512_t const& block512, std::vector<uint32_t>& blocks32)
{
	for (size_t i = 0; i < 16; i++)
	{
		blocks32.push_back((uint32_t)(block512 >> (480 - (i * 32))));
	}
}

uint512_t SHA256::VectorBoolToDecimal(std::vector<bool>& block512)
{
	uint512_t dec_value = 0, base = 1;

	while (!block512.empty())
	{
		if (block512[block512.size() - 1]) dec_value += base;
		base *= 2;
		block512.pop_back();
	}

	return dec_value;
}

std::vector<bool> SHA256::ProcessingPreliminary(std::string const& message)
{
	std::vector<bool> m;
	uint64_t L = 0;

	//step 1
	{
		AddMessageTo(m, message, L);
		m.push_back(true);
	}

	//step 2
	{
		size_t k = GenerateK(L);
		for (size_t i = 0; i < k; i++) m.push_back(false);
	}

	//step 3
	{
		std::string Lbits = DecimalToStringBitSequence(L);
		size_t countOfZeros = 64 - Lbits.size();
		for (size_t i = 0; i < countOfZeros; i++) m.push_back(false);
		for (size_t i = 0; i < Lbits.size(); i++) m.push_back(Lbits[i] == '1');
	}

	return m;
}

std::string SHA256::DecimalToStringBitSequence(uint64_t L)
{
	std::string binaryString;

	while (L)
	{
		if (L & 1) binaryString += '1';
		else binaryString += '0';
		L >>= 1;
	}

	std::reverse(binaryString.begin(), binaryString.end());

	return binaryString;
}

size_t SHA256::GenerateK(uint64_t L)
{
	for (size_t k = 1; ; k++) if ((L + 1 + k) % 512 == 448) return k;
}

void SHA256::AddMessageTo(std::vector<bool>& m, std::string const& message, size_t& lengthBinaryString)
{
	std::string binaryString = StringToStringBitSequence(message);
	lengthBinaryString = binaryString.size();

	for (auto bit : binaryString) m.push_back(bit == '1');
}

std::string SHA256::StringToStringBitSequence(std::string const& message)
{
	std::string binaryString;

	for (auto& symbol : message) binaryString += std::bitset<8>(symbol).to_string();

	return binaryString;
}

