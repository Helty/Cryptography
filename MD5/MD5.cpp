﻿#include "MD5.h"

namespace
{
    WordArray s = 
    {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    WordArray K = 
    {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
    };
}

std::string CMD5::Hashed(std::string const& message)
{
    WordArray wordArray = PadingMessage(message);

    uint32_t A0 = 0x67452301, B0 = 0xefcdab89, C0 = 0x98badcfe, D0 = 0x10325476;

    for (size_t i = 0; i < wordArray.size(); i += 16)
    {
        WordArray M(wordArray.begin() + i, wordArray.begin() + i + 16);

        uint32_t A = A0, B = B0, C = C0, D = D0;

        for (uint32_t i = 0; i < 64; i++)
        {
            uint32_t F = 0, g = 0;

            if (i < 16)
            {
                F = (B & C) | ((~B) & D);
                g = i;
            }
            else if (i < 32)
            {
                F = (D & B) | ((~D) & C);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            }
            else 
            {
                F = C ^ (B | (~D));
                g = (7 * i) % 16;
            }

            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + std::rotl(F, s[i]);
        }

        A0 += A;
        B0 += B;
        C0 += C;
        D0 += D;
    }


    std::string hash;
    std::stringstream sstream;
    sstream << std::setfill('0') << std::setw(8) << std::hex << std::byteswap(A0) << std::byteswap(B0) << std::byteswap(C0) << std::byteswap(D0);
    hash.append(sstream.str());
    std::transform(hash.begin(), hash.end(), hash.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return hash;
}

WordArray CMD5::PadingMessage(std::string const& message)
{
    uint32_t messageLength = static_cast<uint32_t>(message.length());

    uint32_t countBlocks = ((messageLength * 8 + 64) / 512) + 1;
    uint32_t lengthBlocks = countBlocks * 16;
    WordArray result(lengthBlocks);

    for (size_t i = 0; i < messageLength; i++)
    {
        result[i >> 2] |= static_cast<uint32_t>((message[i]) << ((i % 4) * 8));
    }
    result[message.length() >> 2] |= 128 << ((messageLength % 4) * 8);


    result[result.size() - 2] = (messageLength << 3);
    return result;
}
