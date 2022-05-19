#pragma once
#include "string"
#include "vector"
#include <optional>

using ByteArray = std::vector<uint8_t>;
using ByteMatrix = std::vector<ByteArray>;

enum class BLOCK_SIZE
{
	AES128,
	AES192,
	AES256
};

class CAES
{
public:
	CAES() = delete;
	CAES(BLOCK_SIZE const& blockSize);
	
	std::string Encrypt(std::string const& message, std::string const& key) ;
	std::string Decrypt(std::string const& EncryptMessage, std::string const& key);

private:
	ByteArray EncryptBlock(ByteArray const& message, ByteMatrix const& roundKeys);
	ByteArray DecryptBlock(ByteArray const& encryptMessage, ByteMatrix const& roundKeys);
	
	ByteMatrix KeyExpansion(std::string const& key);
	void RotWord(ByteArray&);
	void SubWord(ByteArray&);

	void AddRoundKey(ByteMatrix& state, ByteMatrix const& key, size_t start = 0);
	void SubBytes(ByteMatrix& state);
	void ShiftRows(ByteMatrix& state);
	void ShiftRow(ByteMatrix& state, size_t i, size_t n);
	void MixColumns(ByteMatrix& state);

	void InvShiftRows(ByteMatrix& state);
	void InvSubBytes(ByteMatrix& state);
	void InvMixColumns(ByteMatrix& state);

	ByteArray SplitMatrixToArray(ByteMatrix const& state);
	ByteMatrix ByteArrayToByteMatrix(ByteArray const& message);

private:
	struct SETTINGS 
	{
		size_t Nb;
		size_t Nk;
		size_t Nr;
	};

	size_t m_countAddingBytes = 0;
	SETTINGS m_settings;
};

