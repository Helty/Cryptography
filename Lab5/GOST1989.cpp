#define LSHIFT_nBIT(x, L, N) (((x << L) | (x >> (-L & (N - 1)))) & (((uint64_t)1 << N) - 1))
#define RSHIFT_nBIT(x, R, N) (((x >> R) | (x << (-R & (N - 1)))) & (((uint64_t)1 << N) - 1))
#include "GOST1989.h"

namespace
{
    const std::vector<std::vector<uint8_t>> m_sbox = {
        {0x9, 0x6, 0x3, 0x2, 0x8, 0xB, 0x1, 0x7, 0xA, 0x4, 0xE, 0xF, 0xC, 0x0, 0xD, 0x5},
        {0x3, 0x7, 0xE, 0x9, 0x8, 0xA, 0xF, 0x0, 0x5, 0x2, 0x6, 0xC, 0xB, 0x4, 0xD, 0x1},
        {0xE, 0x4, 0x6, 0x2, 0xB, 0x3, 0xD, 0x8, 0xC, 0xF, 0x5, 0xA, 0x0, 0x7, 0x1, 0x9},
        {0xE, 0x7, 0xA, 0xC, 0xD, 0x1, 0x3, 0x9, 0x0, 0x2, 0xB, 0x4, 0xF, 0x8, 0x5, 0x6},
        {0xB, 0x5, 0x1, 0x9, 0x8, 0xD, 0xF, 0x0, 0xE, 0x4, 0x2, 0x3, 0xC, 0x7, 0xA, 0x6},
        {0x3, 0xA, 0xD, 0xC, 0x1, 0x2, 0x0, 0xB, 0x7, 0x5, 0x9, 0x4, 0x8, 0xF, 0xE, 0x6},
        {0x1, 0xD, 0x2, 0x9, 0x7, 0xA, 0x6, 0x0, 0x8, 0xC, 0x4, 0x5, 0xF, 0x3, 0xB, 0xE},
        {0xB, 0xA, 0xF, 0x5, 0x0, 0xC, 0xE, 0x8, 0x6, 0x2, 0x3, 0x9, 0x1, 0x7, 0xD, 0x4},
    };
}

uint64_t GOST1989::Join32To64(uint32_t const& block32_1, uint32_t const& block32_2)
{
    uint64_t block64;
    block64 = block32_2;
    block64 = (block64 << 32) | block32_1;
    return block64;
}

void GOST1989::Split64To8(uint64_t const& block64, std::vector<uint8_t>& blocks8)
{
    for (size_t i = 0; i < 8; ++i)
    {
        blocks8.push_back((uint8_t)(block64 >> ((7 - i) * 8)));
    }
}

uint32_t GOST1989::Join4To32(std::vector<uint8_t> const& blocks4)
{
    uint32_t block32 = 0;
    for (uint8_t i = 0; i < 4; ++i) block32 = (block32 << 8) | blocks4[i];
    return block32;
}

void GOST1989::SubstitutionTableBy4(std::vector<uint8_t>& blocks4, uint8_t const& sboxRow)
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

void GOST1989::Split32To8(uint32_t const& block32, std::vector<uint8_t>& blocks8)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        blocks8.push_back((uint8_t)(block32 >> (24 - (i * 8))));
    }
}

uint32_t GOST1989::SubstitutionTable(uint32_t const& block32, uint8_t const& sboxRow)
{
    std::vector<uint8_t> blocks4;
    Split32To8(block32, blocks4);
    SubstitutionTableBy4(blocks4, sboxRow);
    return Join4To32(blocks4);
}

uint64_t GOST1989::Join8To64(std::vector<uint8_t> const& blocks8)
{
    uint64_t block64 = 0;
    for (size_t i = 0; i < 8; ++i) block64 = (block64 << 8) | blocks8[i];
    return block64;
}

void GOST1989::Split64To32(uint64_t const& block64, uint32_t& block32_1, uint32_t& block32_2)
{
    block32_2 = (uint32_t)(block64);
    block32_1 = (uint32_t)(block64 >> 32);
}

void GOST1989::Split256To32(std::vector<uint8_t> const& key256, std::vector<uint32_t>& keys32)
{
    size_t countBlock = 0;

    for (size_t i = 0; i < 8; i++)
    {
        uint32_t block32 = 0;
        for (uint8_t j = 0; j < 4; ++j)
        {
            block32 = (block32 << 8) | key256[countBlock++];
        }
        keys32.push_back(block32);
    }
}

void GOST1989::RoundOfFeistelCipher(uint32_t& block32_1, uint32_t& block32_2, std::vector<uint32_t> const& keys32, uint8_t const& round)
{
    uint32_t resultOfIter, temp;
    resultOfIter = (block32_1 + keys32[round % 8]) % UINT32_MAX;
    resultOfIter = SubstitutionTable(resultOfIter, round % 8);
    resultOfIter = (uint32_t)LSHIFT_nBIT(resultOfIter, 11, 32);
    temp = block32_1;
    block32_1 = resultOfIter ^ block32_2;
    block32_2 = temp;
}

void GOST1989::FeistelCipher(CryptMode const& mode, uint32_t& block32_1, uint32_t& block32_2, std::vector<uint32_t> const& keys32)
{
    if (mode == CryptMode::ENCRYPT)
    {
        for (uint8_t round = 0; round < 24; ++round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
        for (uint8_t round = 31; round >= 24; --round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
    }
    else if (mode == CryptMode::DECRYPT)
    {
        for (uint8_t round = 0; round < 8; ++round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
        for (uint8_t round = 31; round >= 8; --round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
    }
}

std::vector<uint8_t> GOST1989::StringToByteArray(std::string const& str)
{
    std::vector<uint8_t> bytes;
    bytes.reserve(std::size(str));

    std::transform(std::begin(str), std::end(str), std::back_inserter(bytes),
        [](char c)
        {
            return static_cast<uint8_t>(c);
        });

    return bytes;
}

std::string GOST1989::ByteArrayToString(std::vector<uint8_t> const& bytes)
{
    return std::string(bytes.begin(), bytes.end());
}

std::string GOST1989::CryptMessage(
    std::string& message,
    std::string const& key,
    CryptMode const& mode
)
{
    std::vector<uint8_t> messageCrypt;
    std::vector<uint32_t> keys32;
    std::vector<uint8_t> key256 = StringToByteArray(key);
    uint32_t N1, N2;

    size_t lengthMessage = message.size() % 8 == 0 ? message.size() : message.size() + (8 - (message.size() % 8));
    while (message.size() != lengthMessage) message.push_back(static_cast<uint8_t>(0));

    Split256To32(key256, keys32);

    for (size_t i = 0; i < lengthMessage; i += 8)
    {
        std::vector<uint8_t> subMessage(message.begin() + i, message.begin() + i + 8);
        Split64To32(Join8To64(subMessage), N1, N2);
        FeistelCipher(mode, N1, N2, keys32);
        Split64To8(Join32To64(N1, N2), messageCrypt);
    }

    return std::string(messageCrypt.begin(), messageCrypt.end());
}

std::string GOST1989::EncryptMessage(std::string& message, std::string const& key)
{
    return CryptMessage(message, key, CryptMode::ENCRYPT);
}

std::string GOST1989::DecryptMessage(std::string& message, std::string const& key)
{
    return CryptMessage(message, key, CryptMode::DECRYPT);
}