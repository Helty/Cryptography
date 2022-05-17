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

uint64_t GOST1989::Join32To64(uint32_t const& N1, uint32_t const& N2)
{
    uint64_t block64;
    // block64b = 10101010101010101010101010101010 = 
    // 0000000000000000000000000000000010101010101010101010101010101010
    block64 = N2;
    // block64b = 
    // = (0000000000000000000000000000000010101010101010101010101010101010 << 32) | 11111111111111111111111111111111 = 
    // = 1010101010101010101010101010101000000000000000000000000000000000 | 11111111111111111111111111111111 = 
    // = 101010101010101010101010101010111111111111111111111111111111111
    block64 = (block64 << 32) | N1;
    return block64;
}

void GOST1989::Split64To8(uint64_t const& block64, std::vector<uint8_t>& blocks8)
{
    for (size_t i = 0; i < 8; ++i)
    {
        // blocks8b[0] = 
        // = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> ((7 - 0) * 8)
        // = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> 56 =
        // = (uint8_t)0000000000000000000000000000000000000000000000000000000000001010 =
        // = 00001010
        blocks8.push_back((uint8_t)(block64 >> (56 - (i * 8))));
    }
}

uint32_t GOST1989::Join4To32(std::vector<uint8_t> const& blocks4)
{
    uint32_t block32 = 0;
    // block64b = 00000000000000000000000000000000
    for (uint8_t i = 0; i < 4; ++i)
    {
        // i = 0
        // (00000000000000000000000000000000 << 8) | 11001100 = 
        // 00000000000000000000000011001100
        // i = 1
        // (00000000000000000000000011001100 << 8) | 11110011 = 
        // 00000000000000001100110000000000 | 11110011 = 
        // 00000000000000001100110011110011
        // ... i < 4 ...
        block32 = (block32 << 8) | blocks4[i];
    }
    return block32;
}

void GOST1989::SubstitutionTableBy4(std::vector<uint8_t>& blocks4, uint8_t const& sboxRow)
{
    uint8_t block4_1, block4_2;

    for (uint8_t i = 0; i < 4; ++i)
    {
        // 10101100 & 0x0F = 00001100
        // 1100 -> 1001
        block4_1 = m_sbox[sboxRow][blocks4[i] & 0x0F];

        // 10101100 >> 4 = 00001010
        // 1010 -> 0111
        block4_2 = m_sbox[sboxRow][blocks4[i] >> 4];

        // 00000111
        blocks4[i] = block4_2;

        // (00000111 << 4) | 1001 = 
        // 01110000 | 1001 = 01111001 
        blocks4[i] = (blocks4[i] << 4) | block4_1;
    }
}

void GOST1989::Split32To8(uint32_t const& block32, std::vector<uint8_t>& blocks8)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        // blocks8b[0] = (uint8_t)10111101000101010100101110100010 >> (24 - (0 * 8)) =
        // = (uint8_t)10101010101010101010101010101010 >> 24 = 
        // = (uint8_t)00000000000000000000000010111101
        // = 10111101
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
    // block64b = 0000000000000000000000000000000000000000000000000000000000000000
    for (size_t i = 0; i < 8; ++i)
    {
        // i = 0
        // (0000000000000000000000000000000000000000000000000000000000000000 << 8) | 11001100 = 
        // 0000000000000000000000000000000000000000000000000000000011001100
        // i = 1
        // (0000000000000000000000000000000000000000000000000000000011001100 << 8) | 11110011 = 
        // 0000000000000000000000000000000000000000000000001100110000000000 | 11110011 = 
        // 0000000000000000000000000000000000000000000000001100110011110011
        // ... i < 8 ...
        block64 = (block64 << 8) | blocks8[i];
    }

    return block64;
}

void GOST1989::Split64To32(uint64_t const& block64, uint32_t& block32_1, uint32_t& block32_2)
{
    block32_2 = (uint32_t)(block64);
    block32_1 = (uint32_t)(block64 >> 32);
}

void GOST1989::Split256To32(std::vector<uint8_t> const& blocks256, std::vector<uint32_t>& blocks32)
{
    size_t countBlock = 0;

    // p32[0] = 00000000000000000000000000000000
    for (size_t i = 0; i < 8; i++)
    {
        uint32_t block32 = 0;

        // 00000000000000000000000000000000 << 8 | 10010010 = 00000000000000000000000010010010
        // 00000000000000000000000010010010 << 8 | 00011110 = 00000000000000001001001000011110
        // 00000000000000001001001000011110 << 8 | 11100011 = 00000000100100100001111011100011
        // 00000000100100100001111011100011 << 8 | 01010101 = 10010010000111101110001101010101

        for (uint8_t j = 0; j < 4; ++j)
        {
            block32 = (block32 << 8) | key256[countBlock++];
        }
        keys32.push_back(block32);
    }
}

void GOST1989::RoundOfFeistelCipher(uint32_t& N1, uint32_t& N2, std::vector<uint32_t> const& keys32, uint8_t const& round)
{
    uint32_t resultOfIter;
    uint32_t Ki = keys32[round % 8];

    // RES = (N1 + Ki) mod 2^32
    resultOfIter = (N1 + Ki) % UINT32_MAX;

    // RES = RES -> Sbox
    resultOfIter = SubstitutionTable(resultOfIter, round % 8);

    // RES = RES << 11
    resultOfIter <<= 11;

    // N1, N2 = (RES xor N2), N1
    N2 ^= resultOfIter;
    std::swap(N1, N2);
}

//keys32 [K0, K1, K2, K3, K4, K5, K6, K7]
void GOST1989::FeistelCipher(CryptMode const& mode, uint32_t& N1, uint32_t& N2, std::vector<uint32_t> const& keys32)
{
    if (mode == CryptMode::ENCRYPT)
    {
        // | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 | K0, K1, K2, K3, K4, K5, K6, K7 |
        for (uint8_t round = 0; round < 24; ++round)
        {
            RoundOfFeistelCipher(N1, N2, keys32, round);
        }

        // | K7, K6, K5, K4, K3, K2, K1, K0 |
        for (uint8_t round = 31; round >= 24; --round)
        {
            RoundOfFeistelCipher(N1, N2, keys32, round);
        }
    }
    else if (mode == CryptMode::DECRYPT)
    {
        // | K0, K1, K2, K3, K4, K5, K6, K7 |
        for (uint8_t round = 0; round < 8; ++round)
        {
            RoundOfFeistelCipher(N1, N2, keys32, round);
        }

        // | K7, K6, K5, K4, K3, K2, K1, K0 | K7, K6, K5, K4, K3, K2, K1, K0 | K7, K6, K5, K4, K3, K2, K1, K0 |
        for (uint8_t round = 31; round >= 8; --round)
        {
            RoundOfFeistelCipher(N1, N2, keys32, round);
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
    std::vector<uint8_t> key256 = StringToByteArray(key);
    uint32_t N1, N2;

    size_t lengthMessage = (message.size() % 8 == 0) ? message.size() : message.size() + (8 - (message.size() % 8));

    if (mode == CryptMode::ENCRYPT) m_numberOfAddedLetters = lengthMessage - message.size();

    while (message.size() != lengthMessage) message.push_back(static_cast<uint8_t>(0));

    std::vector<uint32_t> blocks32;
    Split256To32(key256, blocks32);

    for (size_t i = 0; i < lengthMessage; i += 8)
    {
        std::vector<uint8_t> subMessage(message.begin() + i, message.begin() + i + 8);
        Split64To32(Join8To64(subMessage), N1, N2);
        FeistelCipher(mode, N1, N2, blocks32);
        Split64To8(Join32To64(N1, N2), messageCrypt);
    }

    if (mode == CryptMode::DECRYPT)
    {
        while (m_numberOfAddedLetters != 0)
        {
            messageCrypt.pop_back();
            m_numberOfAddedLetters--;
        }
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