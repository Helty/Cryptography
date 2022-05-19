#include "AES.h"
#include <iostream>
#include <Windows.h>

std::string GetMessageFromUser()
{
    std::string message;

    std::cout << "Введите сообщение которое хотите хешировать: ";
    std::getline(std::cin, message);
    system("cls");
    return message;
}

static inline void PrintBytes(std::string const& str)
{
    std::cout << "[ ";
    for (size_t i = 0; i < str.size(); ++i) std::cout << (uint8_t)str[i] - 0 << " ";
    std::cout << "]" << std::endl;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    CAES aes128(BLOCK_SIZE::AES128);

    std::string message = "Это сообщение должно быть зашифрованно"; //GetMessageFromUser();
    std::string key = "qqqqqqqqqqqqqqqq"; //GetMessageFromUser();

    std::string encryptMessage = aes128.Encrypt(message, key);
    std::string decryptMessage = aes128.Decrypt(encryptMessage, key);

    std::cout << "Исходное сообщение:\t\t"; PrintBytes(message);
    std::cout << "Зашифрованное сообщение:\t"; PrintBytes(encryptMessage);
    std::cout << "Расшифрованное сообщение:\t"; PrintBytes(decryptMessage);
}