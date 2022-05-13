#include "GOSTHash94.h"
#include <bitset>

const std::vector<std::vector<uint8_t>> m_sbox = {
	{0x4, 0xA, 0x9, 0x2, 0xD, 0x8, 0x0, 0xE, 0x6, 0xB, 0x1, 0xC, 0x7, 0xF, 0x5, 0x3},
	{0xE, 0xB, 0x4, 0xC, 0x6, 0xD, 0xF, 0xA, 0x2, 0x3, 0x8, 0x1, 0x0, 0x7, 0x5, 0x9},
	{0x5, 0x8, 0x1, 0xD, 0xA, 0x3, 0x4, 0x2, 0xE, 0xF, 0xC, 0x7, 0x6, 0x0, 0x9, 0xB},
	{0x7, 0xD, 0xA, 0x1, 0x0, 0x8, 0x9, 0xF, 0xE, 0x4, 0x6, 0xC, 0xB, 0x2, 0x5, 0x3},
	{0x6, 0xC, 0x7, 0x1, 0x5, 0xF, 0xD, 0x8, 0x4, 0xA, 0x9, 0xE, 0x0, 0x3, 0xB, 0x2},
	{0x4, 0xB, 0xA, 0x0, 0x7, 0x2, 0x1, 0xD, 0x3, 0x6, 0x8, 0x5, 0x9, 0xC, 0xF, 0xE},
	{0xD, 0xB, 0x4, 0x1, 0x3, 0xF, 0x5, 0x9, 0x0, 0xA, 0xE, 0x7, 0x6, 0x8, 0x2, 0xC},
	{0x1, 0xF, 0xD, 0x0, 0x5, 0x7, 0xA, 0x4, 0x9, 0x2, 0x3, 0xE, 0x6, 0xB, 0x8, 0xC},
};
const std::vector<uint256_t> c_const = {
		0,
		uint256_t("0xff00ffff000000ffff0000ff00ffff0000ff00ff00ff00ffff00ff00ff00ff00"),
		0
};

void Split256To64(uint256_t const& from, std::vector<uint64_t>& to)
{
	for (size_t i = 0; i < 4; ++i)
	{
		to.push_back((uint64_t)(from >> (192 - (i * 64))));
	}
}
void Split256To32(uint256_t const& from, std::vector<uint32_t>& to)
{
	for (size_t i = 0; i < 8; ++i)
	{
		to.push_back((uint32_t)(from >> (224 - (i * 32))));
	}
}
void Split256To16(uint256_t const& from, std::vector<uint16_t>& to)
{
	for (size_t i = 0; i < 16; ++i)
	{
		to.push_back((uint16_t)(from >> (240 - (i * 16))));
	}
}
void Split256To8(uint256_t const& from, std::vector<uint8_t>& to)
{
	for (size_t i = 0; i < 32; ++i)
	{
		to.push_back((uint8_t)(from >> (248 - (i * 8))));
	}
}
void Split64To32(uint256_t const& from, uint32_t& to1, uint32_t& to2)
{
	to2 = (uint32_t)(from);
	to1 = (uint32_t)(from >> 32);
}
void Split32To8(uint32_t const& from, std::vector<uint8_t>& to)
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		to.push_back((uint8_t)(from >> (24 - (i * 8))));
	}
}
uint256_t Join64To256(std::vector<uint64_t> const& from)
{
	uint256_t block256 = 0;
	for (uint64_t i = 0; i < 4; ++i)
	{
		block256 = (block256 << 64) | from[i];
	}
	return block256;
}
uint256_t Join16To256(std::vector<uint16_t> const& from)
{
	uint256_t block256 = 0;
	for (uint64_t i = 0; i < 16; ++i)
	{
		block256 = (block256 << 16) | from[i];
	}
	return block256;
}
uint256_t Join8To256(std::vector<uint8_t> const& from)
{
	uint256_t block256 = 0;
	for (uint64_t i = 0; i < 32; ++i)
	{
		block256 = (block256 << 8) | from[i];
	}
	return block256;
}
uint64_t Join32To64(uint32_t const& N1, uint32_t const& N2)
{
	uint64_t block64;
	block64 = N2;
	block64 = (block64 << 32) | N1;
	return block64;
}
uint32_t Join4To32(std::vector<uint8_t> const& from)
{
	uint32_t block32 = 0;
	for (uint8_t i = 0; i < 4; ++i)
	{
		block32 = (block32 << 8) | from[i];
	}
	return block32;
}

std::string ToStringBitSequence(std::string const& message)
{
	std::string binaryString;

	for (auto& symbol : message) binaryString += std::bitset<8>(symbol).to_string();

	return binaryString;
}
void StringToVectorBool(std::vector<bool> & binarySequence, std::string const& message)
{
	for (auto bit : ToStringBitSequence(message)) binarySequence.push_back(bit == '1');
}
uint256_t VectorBoolToDecimal(std::vector<bool> const& block256)
{
	std::vector<bool> newblock256 = block256;
	uint256_t dec_value = 0, base = 1;

	while (!newblock256.empty())
	{
		if (newblock256[newblock256.size() - 1]) dec_value += base;
		base *= 2;
		newblock256.pop_back();
	}

	return dec_value;
}

void SubstitutionTableBy4(std::vector<uint8_t>& blocks4, uint8_t const& sboxRow)
{
	uint8_t block4_1, block4_2;

	for (uint8_t i = 0; i < 4; ++i)
	{
		block4_1 = m_sbox[sboxRow][blocks4[i] & 0x0F];
		block4_2 = m_sbox[sboxRow][blocks4[i] >> 4];
		blocks4[i] = block4_2;
		blocks4[i] = (blocks4[i] << 4) | block4_1;
	}
}
uint32_t SubstitutionTable(uint32_t const& block32, uint8_t const& sboxRow)
{
	std::vector<uint8_t> blocks4;
	Split32To8(block32, blocks4); //4
	SubstitutionTableBy4(blocks4, sboxRow);
	return Join4To32(blocks4); // 5
}
void RoundOfFeistelCipher(uint32_t& A0, uint32_t& B0, std::vector<uint32_t> const& keys32, uint8_t const& round)
{
	// RES = (N1 + Ki) mod 2^32
	uint32_t resultOfIter = A0 + keys32[round % 8];

	// RES = RES -> Sbox
	resultOfIter = SubstitutionTable(resultOfIter, round % 8);

	// RES = RES << 11
	resultOfIter <<= 11;

	// N1, N2 = (RES xor N2), N1
	B0 ^= resultOfIter;
	std::swap(A0, B0); //6
}
void FeistelCipher(uint32_t& A0, uint32_t& B0, std::vector<uint32_t> const& keys32)
{
	// | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 |
	for (uint8_t round = 0; round < 24; ++round)
	{
		RoundOfFeistelCipher(A0, B0, keys32, round);
	}

	// | K7, K6, K5, K4, K3, K2, K1, K0 |
	for (uint8_t round = 31; round >= 24; --round)
	{
		RoundOfFeistelCipher(A0, B0, keys32, round);
	}
}

uint256_t TransformationA(uint256_t const& Y)
{
	std::vector<uint64_t> y;
	Split256To64(Y, y);

	std::reverse(y.begin(), y.end());

	uint64_t s = y[0] ^ y[1];
	for (size_t j = 0; j < 3; j++) y[j] = y[j + 1];
	y[3] = s;

	std::reverse(y.begin(), y.end());

	return Join64To256(y);
}
uint256_t TransformationP(uint256_t const& Y)
{
	std::vector<uint8_t> y;
	Split256To8(Y, y);

	std::vector<uint8_t> newY(32, 0);

	for (size_t fi = 32; fi > 0; fi--)
	{
		for (size_t i = 0; i <= 3; i++)
		{
			for (size_t k = 1; k <= 8; k++)
			{
				if ((i + 1 + 4 * (k - 1)) == fi)
				{
					size_t pos = 8 * i + k;
					newY[fi - 1] = y[pos - 1];
				}
			}
		}
	}

	return Join8To256(newY);
}
uint256_t TransformationW(uint256_t const& Y, size_t counter)
{
	std::vector<uint16_t> y;
	Split256To16(Y, y);

	std::reverse(y.begin(), y.end());

	for (size_t i = 0; i < counter; i++)
	{
		uint16_t s = (y[0] ^ y[1] ^ y[2] ^ y[3] ^ y[12] ^ y[15]);

		for (size_t j = 0; j < 15; j++) y[j] = y[j + 1];
		y[15] = s;
	}

	std::reverse(y.begin(), y.end());

	return Join16To256(y);
}

void Split256To32Ver(uint256_t const& from, std::vector<uint32_t>& to)
{
	for (size_t i = 0; i < 8; ++i)
	{
		uint32_t temp;
		for (size_t i = 0; i < 4; ++i)
		{
			temp = (uint32_t)(from >> (248 - (i * 8))) & 0xFF;
		}

		to.push_back(temp);
	}
}

uint64_t TransformationE(uint64_t const& h, uint256_t const& key)
{
	uint32_t A, B;
	Split64To32(h, A, B); //2

	std::vector<uint32_t> keys32;
	Split256To32Ver(key, keys32); //3

	FeistelCipher(A, B, keys32);

	return Join32To64(A, B); //7
}

std::vector<uint256_t> KeyGeneration(uint256_t const& hash, uint256_t const& m)
{
	uint256_t U = hash, V = m;
	uint256_t W = U ^ V;
	std::vector<uint256_t> keys;

	keys.push_back(TransformationP(W));

	for (size_t j = 0; j < 3; j++)
	{
		U = TransformationA(U) ^ c_const[j];
		V = TransformationA(TransformationA(V));
		W = U ^ V;
		keys.push_back(TransformationP(W));
	}
	return keys;
}
uint256_t EncryptionTransformation(uint256_t const& hash, std::vector<uint256_t> const& keys)
{
	std::vector<uint64_t> h, s;
	Split256To64(hash, h); //1

	for (size_t i = 0; i < 4; i++)
	{
		std::cout << "Key" << i << ": " << std::hex << keys[i] << std::endl;
		std::cout << "h" << i << ": " << std::hex << h[i] << std::endl;
		s.push_back(TransformationE(h[i], keys[i]));
		std::cout << "s" << i << ": " << std::dec << s[i] << std::endl << std::endl;
	}

	return Join64To256(s); //8
}
uint256_t ShuffleTransformation(uint256_t const& hash, uint256_t const& S, uint256_t const& m)
{
	return TransformationW(hash ^ TransformationW(m ^ TransformationW(S, 12), 1), 61);
}

std::string CGOSTHash94::Hashed(std::string const& message)
{
	uint256_t hash = 0, K = 0, L = 0, m, m2;

	std::vector<bool> binaryMessage;
	std::string invertString = message;
	std::reverse(invertString.begin(), invertString.end());
	StringToVectorBool(binaryMessage, invertString);

	L = binaryMessage.size();

	while (binaryMessage.size() % 256 != 0) binaryMessage.push_back(0);

	for (size_t i = 0; i < binaryMessage.size(); i += 256)
	{
		std::vector<bool> block256(binaryMessage.begin() + i, binaryMessage.begin() + i + 256);

		m = VectorBoolToDecimal(block256);

		Compression(hash, m);
		K += m;
	}

	Compression(hash, L);
	Compression(hash, K);

	std::stringstream sstream;
	sstream << std::hex << hash;

	return sstream.str();
}

void CGOSTHash94::Compression(uint256_t& hash, uint256_t const& m)
{
	std::vector<uint256_t> keys = KeyGeneration(hash, m);
	uint256_t S = EncryptionTransformation(hash, keys);
	hash = ShuffleTransformation(hash, S, m);
}
