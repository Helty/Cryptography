#include "GOSTHash94.h"
#include <bitset>
#include <bit>

namespace 
{
	using ByteArray = std::vector<uint8_t>;
}


const std::vector<ByteArray> m_sbox = {
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
	uint256_t block256 = from[0];
	for (uint64_t i = 1; i < 4; ++i)
	{
		block256 = (block256 << 64) | from[i];
	}
	return block256;
}
uint256_t Join16To256(std::vector<uint16_t> const& from)
{
	uint256_t block256 = from[0];
	for (uint64_t i = 1; i < 16; ++i)
	{
		block256 = (block256 << 16) | from[i];
	}
	return block256;
}
uint256_t Join8To256(std::vector<uint8_t> const& from)
{
	uint256_t block256 = from[0];
	for (uint64_t i = 1; i < 32; ++i)
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

void SubstitutionTableBy4(std::vector<uint8_t>& blocks4)
{
	uint8_t block4_1, block4_2, sboxRow = 0;

	for (uint8_t i = 0; i < 4; ++i)
	{
		block4_1 = m_sbox[sboxRow++][blocks4[i] & 0x0F];
		block4_2 = m_sbox[sboxRow++][blocks4[i] >> 4];
		blocks4[i] = block4_2;
		blocks4[i] = (blocks4[i] << 4) | block4_1;
	}
	uint8_t tmp = blocks4[3];
	blocks4[3] = blocks4[2];
	blocks4[2] = blocks4[1];
	blocks4[1] = blocks4[0];
	blocks4[0] = tmp;
	tmp = blocks4[0] >> 5;
	for (uint8_t i = 1; i < 4; i++)
	{
		uint8_t nTmp = blocks4[i] >> 5;
		blocks4[i] = (blocks4[i] << 3) | tmp;
		tmp = nTmp;
	}
	blocks4[0] = (blocks4[0] << 3) | tmp;
}
uint32_t SubstitutionTable(uint32_t const& block32)
{
	std::vector<uint8_t> blocks4;
	Split32To8(block32, blocks4); //4
	SubstitutionTableBy4(blocks4);
	return Join4To32(blocks4); // 5
}
void RoundOfFeistelCipher(uint32_t& A0, uint32_t& B0, std::vector<uint32_t> const& keys32, uint8_t const& round)
{
	uint32_t resultOfIter = 0, temp = 0;

	for (size_t i = 0; i < 4; i++)
	{
		uint8_t first = static_cast<uint8_t>(A0 >> (24 - (i * 8)));
		uint8_t second = static_cast<uint8_t>(keys32[round % 8] >> (24 - (i * 8)));

		temp += first + second;
		resultOfIter = (resultOfIter << 8) | (temp & 0xFF);
		temp >>= 8;
	}

	//uint32_t resultOfIter = (A0 + keys32[round % 8]) % UINT32_MAX;

	// RES = RES -> Sbox
	resultOfIter = SubstitutionTable(resultOfIter);

	// RES = RES << 11
	//resultOfIter = std::rotl(resultOfIter, 11);

	// N1, N2 = (RES xor N2), N1
	resultOfIter ^= B0;
	B0 = A0;
	A0 = resultOfIter;
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

	for (size_t i = 0; i < counter; i++)
	{
		uint16_t s = (y[0] ^ y[1] ^ y[2] ^ y[3] ^ y[12] ^ y[15]);

		for (size_t j = 0; j < 15; j++) y[j] = y[j + 1];
		y[15] = s;
	}

	return Join16To256(y);
}

void Split256To32Inv(uint256_t const& from, std::vector<uint32_t>& to)
{
	for (size_t i = 0; i < 8; ++i)
	{
		uint32_t res = 0;
		uint32_t temp32 = (uint32_t)(from >> (i * 32));

		for (size_t i = 0; i < 4; ++i)
		{
			uint8_t temp8 = (uint8_t)(temp32 >> (i * 8));
			res = (res << 8) | temp8;
		}

		to.push_back(res);
	}
}

uint64_t TransformationE(uint64_t const& h, uint256_t const& key)
{
	uint32_t A, B;
	Split64To32(h, A, B);

	std::vector<uint32_t> keys32;
	Split256To32Inv(key, keys32);

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
		s.push_back(TransformationE(h[i], keys[i]));
		std::cout << "s" << i << ": " << std::hex << s[i] << std::endl << std::endl;

	}

	return Join64To256(s); //8
}
void ShuffleTransformation(uint256_t& hash, uint256_t const& S, uint256_t const& m)
{
	uint256_t first = TransformationW(S, 12);
	std::cout << "first" << ": " << std::hex << first << std::endl << std::endl;

	uint256_t second = m ^ first;
	std::cout << "second после xor" << ": " << std::hex << second << std::endl << std::endl;
	second = TransformationW(second, 1);
	std::cout << "second после W" << ": " << std::hex << second << std::endl << std::endl;
	uint256_t thrid = hash ^ second;
	std::cout << "thrid после xor" << ": " << std::hex << thrid << std::endl << std::endl;
	thrid = TransformationW(thrid, 61);
	std::cout << "thrid после W" << ": " << std::hex << thrid << std::endl << std::endl;
	hash = thrid;
}

std::string CGOSTHash94::Hashed(std::string const& message)
{
	uint256_t hash = 0, Sum = 0, m;

	std::vector<bool> binaryMessage;
	std::string invertString = message;
	std::reverse(invertString.begin(), invertString.end());
	StringToVectorBool(binaryMessage, invertString);

	while (binaryMessage.size() % 256 != 0) binaryMessage.push_back(0);

	size_t L = binaryMessage.size();

	for (size_t i = 0; i < binaryMessage.size(); i += 256)
	{
		std::vector<bool> block256(binaryMessage.begin() + i, binaryMessage.begin() + i + 256);

		m = VectorBoolToDecimal(block256);

		Sum += m;

		Compression(hash, m);
	}

	Compression(hash, L);
	Compression(hash, Sum);

	std::stringstream sstream;
	sstream << std::hex << hash;

	return sstream.str();
}

void CGOSTHash94::Compression(uint256_t& hash, uint256_t const& m)
{
	std::vector<uint256_t> keys = KeyGeneration(hash, m);
	std::vector<uint8_t> hashNew;

	std::cout << "k0" << ": " << std::hex << keys[0] << std::endl << std::endl;
	std::cout << "k1" << ": " << std::hex << keys[1] << std::endl << std::endl;
	std::cout << "k2" << ": " << std::hex << keys[2] << std::endl << std::endl;
	std::cout << "k3" << ": " << std::hex << keys[3] << std::endl << std::endl;

	Split256To8(hash, hashNew);
	std::reverse(hashNew.begin(), hashNew.end());
	hash = Join8To256(hashNew);

	uint256_t S = EncryptionTransformation(hash, keys);
	std::cout << "S" << ": " << std::hex << S << std::endl << std::endl;
	std::cout << "m" << ": " << std::hex << m << std::endl << std::endl;
	
	std::vector<uint8_t> mNew;
	Split256To8(m, mNew);
	std::reverse(mNew.begin(), mNew.end());
	uint256_t mNew256 = Join8To256(mNew);

	ShuffleTransformation(hash, S, mNew256);

	Split256To8(hash, hashNew);
	std::reverse(hashNew.begin(), hashNew.end());
	hash = Join8To256(hashNew);

	std::cout << "hash" << ": " << std::hex << hash << std::endl << std::endl;
}
