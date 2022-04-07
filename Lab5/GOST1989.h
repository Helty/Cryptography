#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class GOST1989
{
public:
    std::string EncryptMessage(std::string& message, std::string const& key);
    std::string DecryptMessage(std::string& message, std::string const& key);

private:
    size_t m_numberOfAddedLetters;

    enum class CryptMode
    {
        ENCRYPT = 0,
        DECRYPT = 1
    };

    void Split32To8(uint32_t const& block32, std::vector<uint8_t>& blocks8);

    void Split64To8(uint64_t const& block64, std::vector<uint8_t>& blocks8);

    void Split64To32(uint64_t const& block64, uint32_t& block32_1, uint32_t& block32_2);

    void Split256To32(std::vector<uint8_t> const& key256, std::vector<uint32_t>& keys32);

    uint32_t Join4To32(std::vector<uint8_t> const& blocks4);

    uint64_t Join8To64(std::vector<uint8_t> const& blocks8);

    uint64_t Join32To64(uint32_t const& block32_1, uint32_t const& block32_2);

    std::vector<uint8_t> StringToByteArray(std::string const& str);

    std::string ByteArrayToString(std::vector<uint8_t> const& str);

    uint32_t LeftShift(uint32_t const& x);

    uint32_t RightShift(uint32_t const& x);

    void SubstitutionTableBy4(std::vector<uint8_t>& blocks4, uint8_t const& sboxRow);

    uint32_t SubstitutionTable(uint32_t const& block32, uint8_t const& sboxRow);

    void RoundOfFeistelCipher(uint32_t& block32_1, uint32_t& block32_2, std::vector<uint32_t> const& keys32, uint8_t const& round);

    void FeistelCipher(CryptMode const& mode, uint32_t& block32_1, uint32_t& block32_2, std::vector<uint32_t> const& keys32);

    std::string CryptMessage(std::string& message, std::string const& key, CryptMode const& mode);
};

