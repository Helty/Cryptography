#include "GOSTHash94.h"

namespace 
{
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

	const std::vector<ByteArray> c_const = {
			ByteArray(32, 0),
			{
				0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
				0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
				0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF,
				0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF
			},
			ByteArray(32, 0)
	};
}

void CGOSTHash94::Split256To32Inv(ByteArray const& from, std::vector<ByteArray>& to)
{
	for (size_t step = 0; step < 8; step++)
	{
		ByteArray temp4bytes(from.begin() + (step * 4), from.begin() + (step * 4) + 4);
		to.push_back(temp4bytes);
	}
}
ByteArray CGOSTHash94::Split256ToByteArray(uint256_t const& dec)
{
	ByteArray result;

	for (size_t i = 0; i < 32; i++)
	{
		result.push_back(
			static_cast<uint8_t>(dec >> (i * 8))
		);
	}

	return result;
}
ByteArray CGOSTHash94::StringToByteArray(std::string const& str)
{
	ByteArray result;

	std::for_each(str.begin(), str.end(), [&result](unsigned char const& byte)
		{
			result.push_back(byte);
		}
	);

	return result;
}

void CGOSTHash94::TransformationW(ByteArray& Y, size_t counter)
{
	while (counter--)
	{
		std::pair<uint8_t, uint8_t> block16bytes = { 0, 0 };

		block16bytes.first ^= Y[0];  block16bytes.second ^= Y[1];
		block16bytes.first ^= Y[2];  block16bytes.second ^= Y[3];
		block16bytes.first ^= Y[4];  block16bytes.second ^= Y[5];
		block16bytes.first ^= Y[6];  block16bytes.second ^= Y[7];
		block16bytes.first ^= Y[24]; block16bytes.second ^= Y[25];
		block16bytes.first ^= Y[30]; block16bytes.second ^= Y[31];

		for (size_t step = 0; step < 30; step++) Y[step] = Y[step + 2];

		Y[30] = block16bytes.first;
		Y[31] = block16bytes.second;
	}
}
ByteArray CGOSTHash94::TransformationE(ByteArray const& h, ByteArray const& key)
{
	ByteArray A(h.begin(), h.end() - 4);
	ByteArray B(h.begin() + 4, h.end());

	std::vector<ByteArray> keys32;
	Split256To32Inv(key, keys32);

	FeistelCipher(A, B, keys32);

	B.insert(B.end(), A.begin(), A.end());
	return B;
}
ByteArray CGOSTHash94::TransformationP(ByteArray const& Y)
{
	ByteArray result;
	for (size_t i = 0; i < 32; i++) result.push_back(Y[fi(i)]);
	return result;
}
ByteArray CGOSTHash94::TransformationA(ByteArray const& Y)
{
	ByteArray result;
	for (size_t i = 0; i < 24; i++) result.push_back(Y[i + 8]);
	for (size_t i = 0; i < 8; i++) result.push_back(Y[i] ^ Y[i + 8]);
	return result;
}

void CGOSTHash94::ShuffleTransformation(ByteArray& hash, ByteArray& S, ByteArray const& M)
{
	TransformationW(S, 12);
	for (size_t i = 0; i < 32; i++) S[i] ^= M[i];
	TransformationW(S, 1);
	for (size_t i = 0; i < 32; i++) S[i] ^= hash[i];
	TransformationW(S, 61);
	hash = S;
}

void CGOSTHash94::SubstitutionTable(ByteArray& blocks4bytes)
{
	uint8_t block4_1, block4_2, sboxRow = 0;

	for (uint8_t i = 0; i < 4; ++i)
	{
		block4_1 = m_sbox[sboxRow++][blocks4bytes[i] & 0x0F];
		block4_2 = m_sbox[sboxRow++][blocks4bytes[i] >> 4];
		blocks4bytes[i] = block4_2;
		blocks4bytes[i] = (blocks4bytes[i] << 4) | block4_1;
	}
	uint8_t tmp = blocks4bytes[3];
	blocks4bytes[3] = blocks4bytes[2];
	blocks4bytes[2] = blocks4bytes[1];
	blocks4bytes[1] = blocks4bytes[0];
	blocks4bytes[0] = tmp;
	tmp = blocks4bytes[0] >> 5;

	for (uint8_t i = 1; i < 4; i++)
	{
		uint8_t nTmp = blocks4bytes[i] >> 5;
		blocks4bytes[i] = (blocks4bytes[i] << 3) | tmp;
		tmp = nTmp;
	}

	blocks4bytes[0] = (blocks4bytes[0] << 3) | tmp;
}
void CGOSTHash94::RoundOfFeistelCipher(ByteArray& A0, ByteArray& B0, ByteArray const& keys32)
{
	ByteArray resultOfIter;

	uint32_t temp = 0;

	for (size_t i = 0; i < 4; i++)
	{
		temp += A0[i] + keys32[i];
		resultOfIter.push_back(temp & 0xFF);
		temp >>= 8;
	}

	SubstitutionTable(resultOfIter);

	// N1, N2 = (RES xor N2), N1
	for (size_t i = 0; i < 4; i++) resultOfIter[i] ^= B0[i];
	B0 = A0;
	A0 = resultOfIter;
}
void CGOSTHash94::FeistelCipher(ByteArray& A0, ByteArray& B0, std::vector<ByteArray> const& keys32)
{
	// | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 |
	for (uint8_t round = 0; round < 24; ++round)
	{
		RoundOfFeistelCipher(A0, B0, keys32[round % 8]);
	}

	// | K7, K6, K5, K4, K3, K2, K1, K0 |
	for (uint8_t round = 31; round >= 24; --round)
	{
		RoundOfFeistelCipher(A0, B0, keys32[round % 8]);
	}
}
ByteArray CGOSTHash94::EncryptionTransformation(ByteArray const& hash, std::vector<ByteArray> const& keys)
{
	ByteArray S, s;

	for (size_t step = 0; step < 4; step++)
	{
		ByteArray block8Bytes(hash.begin() + (step * 8), hash.begin() + (step * 8) + 8);
		s = TransformationE(block8Bytes, keys[step]);
		S.insert(S.end(), s.begin(), s.end());
	}

	return S;
}

size_t CGOSTHash94::fi(size_t const& arg)
{
	size_t i = (arg & 3), k = (arg >> 2);
	return (i << 3) + (++k) - 1;
}
std::vector<ByteArray> CGOSTHash94::KeyGeneration(ByteArray const& hash, ByteArray const& m)
{
	std::vector<ByteArray> keys;
	ByteArray W, C, U = hash, V = m;

	for (size_t i = 0; i < 32; i++) W.push_back(U[i] ^ V[i]);
	keys.push_back(TransformationP(W));

	for (size_t j = 0; j < 3; j++)
	{
		ByteArray tmp = TransformationA(U);

		for (size_t i = 0; i < 32; i++) U[i] = tmp[i] ^ c_const[j][i];

		V = TransformationA(TransformationA(V));

		for (size_t i = 0; i < 32; i++) W[i] = U[i] ^ V[i];

		keys.push_back(TransformationP(W));
	}

	return keys;
}

void CGOSTHash94::Compression(ByteArray& hash, ByteArray const& m)
{
	std::vector<ByteArray> keys = KeyGeneration(hash, m);

	ByteArray S = EncryptionTransformation(hash, keys);
	
	ShuffleTransformation(hash, S, m);
}

std::string CGOSTHash94::Hashed(std::string const& message)
{
	ByteArray hash(32, 0), Sum(32, 0), m;
	ByteArray messageByteArray = StringToByteArray(message);
	ByteArray length = Split256ToByteArray(messageByteArray.size() * 8);

	while (messageByteArray.size() % 32 != 0) messageByteArray.push_back(0);

	for (size_t step = 0; step < messageByteArray.size(); step += 32)
	{
		ByteArray block32Bytes(messageByteArray.begin() + step, messageByteArray.begin() + step + 32);

		m = block32Bytes;

		uint16_t c = 0;
		for (size_t i = 0; i < 32; i++)
		{
			c += m[i] + Sum[i];
			Sum[i] = c & 0xFF;
			c >>= 8;
		}

		Compression(hash, m);
	}

	Compression(hash, length);
	Compression(hash, Sum);

	std::stringstream sstream;
	for (size_t i = 0; i < 32; i++) sstream << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
	return sstream.str();
}