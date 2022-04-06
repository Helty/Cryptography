#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// 10101100 << 2 = 10110000 | 00000010 = 10110010
#define LSHIFT_nBIT(x, L, N) (((x << L) | (x >> (-L & (N - 1)))) & (((uint64_t)1 << N) - 1))
#define RSHIFT_nBIT(x, R, N) (((x >> R) | (x << (-R & (N - 1)))) & (((uint64_t)1 << N) - 1))
#define BUFF_SIZE 1024
// 1 | 4 -> 0xC 

namespace
{
    using Byte8Array = std::vector<uint8_t>;
    using Byte32Array = std::vector<uint32_t>;
    using namespace boost::multiprecision;

    enum class CryptMod
    {
        ENCRYPT = 0,
        DECRYPT = 1
    };

    static const std::vector<std::vector<uint8_t>> Sbox = {
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

static inline void PrintArray(Byte8Array const& array)
{
    std::cout << "[ ";
    for (size_t i = 0; i < array.size(); ++i) std::cout << array[i] << " ";
    std::cout << "]" << std::endl;
}
uint64_t Join32To64(uint32_t const& block32_1, uint32_t const& block32_2)
{
    uint64_t block64;
    // block64b = 10101010101010101010101010101010 = 
    // 0000000000000000000000000000000010101010101010101010101010101010
    block64 = block32_2;
    // block64b = 
    // = (0000000000000000000000000000000010101010101010101010101010101010 << 32) | 11111111111111111111111111111111 = 
    // = 1010101010101010101010101010101000000000000000000000000000000000 | 11111111111111111111111111111111 = 
    // = 101010101010101010101010101010111111111111111111111111111111111
    block64 = (block64 << 32) | block32_1;
    return block64;
}
void Split64To8(uint64_t const& block64, Byte8Array& blocks8)
{
    for (size_t i = 0; i < 8; ++i)
    {
        // blocks8b[0] = 
        // = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> ((7 - 0) * 8)
        // = (uint8_t)0000101010101010101010101010101010101010101010101010101010101111 >> 56 =
        // = (uint8_t)0000000000000000000000000000000000000000000000000000000000001010 =
        // = 00001010
        blocks8[i] = (uint8_t)(block64 >> ((7 - i) * 8));
    }
}
uint32_t Join4To32(Byte8Array const& blocks4)
{
    uint32_t block32;
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
void SubstitutionTableBy4(Byte8Array& blocks4, uint8_t const& sboxRow)
{
    uint8_t block4_1, block4_2;

    for (uint8_t i = 0; i < 4; ++i)
    {
        // 10101100 & 0x0F = 00001100
        // [example get from table] 1100 -> 1001
        block4_1 = Sbox[sboxRow][blocks4[i] & 0x0F];

        // 10101100 >> 4 = 00001010
        // [example get from table] 1010 -> 0111
        block4_2 = Sbox[sboxRow][blocks4[i] >> 4];

        // 00001001
        blocks4[i] = block4_2;

        // (00001001 << 4) | 0111 = 
        // 1001000 | 0111 = 10010111 
        blocks4[i] = (blocks4[i] << 4) | block4_1;
    }
}
void Split32To8(uint32_t const& block32, Byte8Array& blocks8)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        // blocks8b[0] = (uint8_t)10111101000101010100101110100010 >> (28 - (0 * 8)) =
        // = (uint8_t)10101010101010101010101010101010 >> 28 = 
        // = (uint8_t)00000000000000000000000010111101
        // = 10111101
        blocks8[i] = (uint8_t)(block32 >> (24 - (i * 8)));
    }
}
uint32_t SubstitutionTable(uint32_t const& block32, uint8_t const& sboxRow)
{
    Byte8Array blocks4;
    Split32To8(block32, blocks4);
    SubstitutionTableBy4(blocks4, sboxRow);
    return Join4To32(blocks4);
}
void RoundOfFeistelCipher(uint32_t& block32_1, uint32_t& block32_2, Byte32Array const& keys32, uint8_t const& round)
{
    uint32_t resultOfIter, temp;

    // RES = (N1 + Ki) mod 2^32
    resultOfIter = (block32_1 + keys32[round % 8]) % UINT32_MAX;

    // RES = RES -> Sbox
    resultOfIter = SubstitutionTable(resultOfIter, round % 8);

    // RES = RES <<< 11
    resultOfIter = (uint32_t)LSHIFT_nBIT(resultOfIter, 11, 32);

    // N1, N2 = (RES xor N2), N1
    temp = block32_1;
    block32_1 = resultOfIter ^ block32_2;
    block32_2 = temp;
}
void FeistelCipher(CryptMod const& mode, uint32_t& block32_1, uint32_t& block32_2, Byte32Array const& keys32)
{
    if (mode == CryptMod::ENCRYPT)
    {
        // K0, K1, K2, K3, K4, K5, K6, K7, K0, K1, K2, K3, K4, K5, K6, K7, K0, K1, K2, K3, K4, K5, K6, K7
        for (uint8_t round = 0; round < 24; ++round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }

        // K7, K6, K5, K4, K3, K2, K1, K0
        for (uint8_t round = 31; round >= 24; --round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
    }
    else if (mode == CryptMod::DECRYPT)
    {
        // K0, K1, K2, K3, K4, K5, K6, K7
        for (uint8_t round = 0; round < 8; ++round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }

        // K7, K6, K5, K4, K3, K2, K1, K0, K7, K6, K5, K4, K3, K2, K1, K0, K7, K6, K5, K4, K3, K2, K1, K0
        for (uint8_t round = 31; round >= 8; --round)
        {
            RoundOfFeistelCipher(block32_1, block32_2, keys32, round);
        }
    }
}
uint64_t Join8To64(Byte8Array const& blocks8)
{
    uint64_t block64;
    for (size_t i = 0; i < 8; ++i) block64 = (block64 << 8) | blocks8[i];
    return block64;
}
void Split64To32(uint64_t const& block64, uint32_t& block32_1, uint32_t& block32_2)
{
    block32_2 = (uint32_t)(block64);
    block32_1 = (uint32_t)(block64 >> 32);
}
void Split256To32(Byte8Array const& key256, Byte32Array& keys32)
{
    size_t countBlock = 0;

    for (size_t i = 0; i < 8; i++)
    {
        uint32_t block32;
        for (uint8_t j = 0; j < 4; ++j)
        {
            block32 = (block32 << 8) | key256[countBlock++];
        }
        keys32.push_back(block32);
    }
}
size_t GOST1989(
    Byte8Array& messageCrypt,
    CryptMod const& mode,
    Byte8Array const& key,
    Byte8Array const& message,
    size_t lengthMessage
)
{
    lengthMessage = lengthMessage % 8 == 0 ? lengthMessage : lengthMessage + (8 - (lengthMessage % 8));
    uint32_t N1, N2;
    Byte32Array keys32;
    Split256To32(key, keys32);

    for (size_t i = 0; i < lengthMessage; i += 8)
    {
        Byte8Array subMessage(&message[i], &message[i + 8]);

        Split64To32(Join8To64(subMessage), N1, N2);

        FeistelCipher(mode, N1, N2, keys32);
        
        Split64To8(Join32To64(N1, N2), messageCrypt);
    }

    return lengthMessage;
}
Byte8Array StringToByte8Array(std::string const& str)
{
    Byte8Array bytes;
    bytes.reserve(std::size(str));

    std::transform(std::begin(str), std::end(str), std::back_inserter(bytes),
        [](char c)
        {
            return static_cast<uint8_t>(c);
        });

    return bytes;
}
std::string Byte8ArrayToString(Byte8Array const& bytes)
{
    return reinterpret_cast<const char*>(bytes.data());
}
std::string GetMessageFromUser()
{
    std::string message;
    std::cout << "Please, enter your message for encrypt: ";
    std::getline(std::cin, message);
    return message;
}
void StartGOST1989()
{
    Byte8Array openMessage = StringToByte8Array(GetMessageFromUser());
    size_t sizeOfOpenMessage = openMessage.size();
    Byte8Array key256b = StringToByte8Array("bkvzhzkyzelebdbyvujamjewljkwomtz");

    Byte8Array encryptedMessage;
    Byte8Array decryptedMessage;

    std::cout << "Open message: " << std::endl;
    PrintArray(openMessage);
    std::cout << Byte8ArrayToString(openMessage) << std::endl;

    size_t sizeOfEncryptedMessage = GOST1989(encryptedMessage, CryptMod::ENCRYPT, key256b, openMessage, sizeOfOpenMessage);
    
    std::cout << "Encrypted message: " << std::endl;
    PrintArray(encryptedMessage);
    std::cout << Byte8ArrayToString(encryptedMessage) << std::endl;

    size_t sizeOfDecryptedMessage = GOST1989(decryptedMessage, CryptMod::DECRYPT, key256b, encryptedMessage, sizeOfEncryptedMessage);

    std::cout << "Decrypted message: " << std::endl;
    PrintArray(decryptedMessage);
    std::cout << Byte8ArrayToString(decryptedMessage) << std::endl;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        StartGOST1989();
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}