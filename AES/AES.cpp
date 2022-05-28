#include "AES.h"
#include <algorithm>
#include <iostream>

namespace
{
    const ByteMatrix S_BOX = {
        {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
        {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
        {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
        {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
        {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
        {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
        {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
        {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
        {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
        {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
        {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
        {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
        {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
        {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
        {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
        {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
    };

    const ByteMatrix INV_S_BOX = {
        {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
    };

    const ByteMatrix R_CON = {
        {0x00, 0x00, 0x00, 0x00},
        {0x01, 0x00, 0x00, 0x00},
        {0x02, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00},
        {0x08, 0x00, 0x00, 0x00},
        {0x10, 0x00, 0x00, 0x00},
        {0x20, 0x00, 0x00, 0x00},
        {0x40, 0x00, 0x00, 0x00},
        {0x80, 0x00, 0x00, 0x00},
        {0x1b, 0x00, 0x00, 0x00},
        {0x36, 0x00, 0x00, 0x00}
    };

    const ByteMatrix PREDICATE_MATRIX =
    {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}
    };

    const ByteMatrix INV_PREDICATE_MATRIX = 
    {
        {14, 11, 13, 9},
        {9, 14, 11, 13},
        {13, 9, 14, 11},
        {11, 13, 9, 14}
    };

    ByteArray StringToByteArray(std::string const& str)
    {
        ByteArray result;

        std::for_each(str.begin(), str.end(), [&result](unsigned char const& byte)
            {
                result.push_back(byte);
            }
        );

        return result;
    }

    uint8_t GFM(uint8_t a, uint8_t b)
    {
        uint8_t result = 0;

        for (size_t counter = 0; counter < 8; counter++)
        {
            if ((b & 1) != 0) result ^= a;

            bool hi_bit_set = ((a & 128) != 0);
            a <<= 1;
            if (hi_bit_set) a ^= 27; /* x^8 + x^4 + x^3 + x + 1 */
            b >>= 1;
        }

        return result;
    }

    const ByteArray operator ^ (ByteArray const& first, ByteArray const& second)
    {
        ByteArray result;

        for (size_t i = 0; i < first.size(); i++) result.push_back(first[i] ^ second[i]);

        return result;
    }

    ByteArray operator ^ (ByteMatrix const& first, ByteArray const& second)
    {
        ByteArray result = 
        {
            (uint8_t)(GFM(second[0], first[0][0]) ^ GFM(second[1], first[0][1]) ^ GFM(second[2], first[0][2]) ^ GFM(second[3], first[0][3])),
            (uint8_t)(GFM(second[0], first[1][0]) ^ GFM(second[1], first[1][1]) ^ GFM(second[2], first[1][2]) ^ GFM(second[3], first[1][3])),
            (uint8_t)(GFM(second[0], first[2][0]) ^ GFM(second[1], first[2][1]) ^ GFM(second[2], first[2][2]) ^ GFM(second[3], first[2][3])),
            (uint8_t)(GFM(second[0], first[3][0]) ^ GFM(second[1], first[3][1]) ^ GFM(second[2], first[3][2]) ^ GFM(second[3], first[3][3]))
        };

        return result;
    }
}

CAES::CAES(BLOCK_SIZE const& blockSize)
{
    m_settings.Nb = 4;

    if (blockSize == BLOCK_SIZE::AES128)
    {
        m_settings.Nk = 4;
        m_settings.Nr = 10;
    }
    else if (blockSize == BLOCK_SIZE::AES192)
    {
        m_settings.Nk = 6;
        m_settings.Nr = 12;
    }
    else if (blockSize == BLOCK_SIZE::AES256)
    {
        m_settings.Nk = 8;
        m_settings.Nr = 14;
    }
}

ByteArray CAES::SplitMatrixToArray(ByteMatrix const& state)
{
    ByteArray byteArray(16, 0);

    for (size_t r = 0; r < 4; r++)
    {
        for (size_t c = 0; c < m_settings.Nb; c++)
        {
            byteArray[r + 4 * c] = state[r][c];
        }
    }

    return byteArray;
}
ByteMatrix CAES::ByteArrayToByteMatrix(ByteArray const& byteArray)
{
    ByteMatrix state(4, ByteArray(m_settings.Nb, 0));

    for (size_t r = 0; r < 4; r++)
    {
        for (size_t c = 0; c < m_settings.Nb; c++)
        {
            state[r][c] = byteArray[r + 4 * c];
        }
    }

    return state;
}

void CAES::RotWord(ByteArray& arrayBytes)
{
    arrayBytes.push_back(arrayBytes[0]);
    arrayBytes.erase(arrayBytes.begin());
}
void CAES::SubWord(ByteArray& arrayBytes)
{
    std::for_each(arrayBytes.begin(), arrayBytes.end(), [&arrayBytes](uint8_t& byte)
        {
            byte = S_BOX[byte / 16][byte % 16];
        }
    );
}

ByteMatrix CAES::KeyExpansion(std::string const& key)
{
    ByteMatrix result(m_settings.Nb * (m_settings.Nr + 1), ByteArray(4, 0));
    ByteArray keyBytes = StringToByteArray(key), temp(4, 0);

    size_t i = 0;
    while (i < m_settings.Nk)
    {
        result[i] = { keyBytes[4 * i], keyBytes[4 * i + 1], keyBytes[4 * i + 2], keyBytes[4 * i + 3] };
        i++;
    }

    i = m_settings.Nk;
    while (i < m_settings.Nb * (m_settings.Nr + 1))
    {
        temp = result[i - 1];

        if (i % m_settings.Nk == 0)
        {
            RotWord(temp);
            SubWord(temp);
            temp = temp ^ R_CON[i / m_settings.Nk];
        }
        else if ((m_settings.Nk > 6) and (i % m_settings.Nk == 4))
        {
            SubWord(temp);
        }

        result[i] = result[i - m_settings.Nk] ^ temp;
        i++;
    }

    return result;
}

std::string CAES::Encrypt(std::string const& message, std::string const& key)
{
    if (key.size() % 16 != 0) throw std::exception("Error key.");

    ByteMatrix roundKeys = KeyExpansion(key);
    ByteArray result, messageBytes = StringToByteArray(message);

    while (messageBytes.size() % 16 != 0)
    {
        messageBytes.push_back(0);
        m_countAddingBytes++;
    }

    for (size_t i = 0; i < messageBytes.size(); i += 16)
    {
        ByteArray in(messageBytes.begin() + i, messageBytes.begin() + i + 16);
        ByteArray out = EncryptBlock(in, roundKeys);
        result.insert(result.end(), out.begin(), out.end());
    }

    return std::string(result.begin(), result.end());
}
std::string CAES::Decrypt(std::string const& EncryptMessage, std::string const& key)
{
    if (key.size() % 16 != 0) throw std::exception("Error key.");

    ByteMatrix roundKeys = KeyExpansion(key);
    ByteArray result, messageBytes = StringToByteArray(EncryptMessage);

    for (size_t i = 0; i < messageBytes.size(); i += 16)
    {
        ByteArray in(messageBytes.begin() + i, messageBytes.begin() + i + 16);
        ByteArray out = DecryptBlock(in, roundKeys);
        result.insert(result.end(), out.begin(), out.end());
    }

    while (m_countAddingBytes)
    {
        result.pop_back(); 
        m_countAddingBytes--;
    }

    return std::string(result.begin(), result.end());
}

ByteArray CAES::EncryptBlock(ByteArray const& messageBytes, ByteMatrix const& roundKeys)
{
    ByteMatrix state = ByteArrayToByteMatrix(messageBytes);

    AddRoundKey(state, roundKeys);

    for (size_t round = 1; round <= m_settings.Nr - 1; round++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumnsTemplate(state, PREDICATE_MATRIX);
        AddRoundKey(state, roundKeys, (round * m_settings.Nb));
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys, (m_settings.Nr * m_settings.Nb));

    return SplitMatrixToArray(state);
}
ByteArray CAES::DecryptBlock(ByteArray const& encryptMessageBytes, ByteMatrix const& roundKeys)
{
    ByteMatrix state = ByteArrayToByteMatrix(encryptMessageBytes);

    AddRoundKey(state, roundKeys, (m_settings.Nr * m_settings.Nb));

    for (size_t round = m_settings.Nr - 1; round >= 1; round--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKeys, (round * m_settings.Nb));
        MixColumnsTemplate(state, INV_PREDICATE_MATRIX);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKeys);

    return SplitMatrixToArray(state);
}

void CAES::AddRoundKey(ByteMatrix& state, ByteMatrix const& key, size_t start)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < m_settings.Nb; j++)
        {
            state[i][j] ^= key[start + j][i];
        }
    }
}

void CAES::ShiftRow(ByteMatrix& state, size_t i, size_t n)
{
    ByteArray tmp;

    for (size_t j = 0; j < m_settings.Nb; j++)
    {
        tmp.push_back(state[i][(j + n) % m_settings.Nb]);
    }

    state[i] = tmp;
}
void CAES::ShiftRows(ByteMatrix& state)
{
    ShiftRow(state, 1, 1);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 3);
}
void CAES::SubBytes(ByteMatrix& state)
{
    uint8_t t;

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < m_settings.Nb; j++)
        {
            t = state[i][j];
            state[i][j] = S_BOX[t / 16][t % 16];
        }
    }
}

void CAES::InvShiftRows(ByteMatrix& state)
{
    ShiftRow(state, 1, m_settings.Nb - 1);
    ShiftRow(state, 2, m_settings.Nb - 2);
    ShiftRow(state, 3, m_settings.Nb - 3);
}
void CAES::InvSubBytes(ByteMatrix& state)
{
    uint8_t t;
    for (size_t i = 0; i < 4; i++) 
    {
        for (size_t j = 0; j < m_settings.Nb; j++) 
        {
            t = state[i][j];
            state[i][j] = INV_S_BOX[t / 16][t % 16];
        }
    }
}

void CAES::MixColumnsTemplate(ByteMatrix& state, ByteMatrix const& predicatMatrix)
{
    ByteArray result(4);

    for (size_t row = 0; row < 4; row++)
    {
        ByteArray currentColum = { 
            state[0][row],
            state[1][row],
            state[2][row],
            state[3][row]
        };

        result = predicatMatrix ^ currentColum;

        state[0][row] = result[0];
        state[1][row] = result[1];
        state[2][row] = result[2];
        state[3][row] = result[3];
    }
}