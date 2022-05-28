#include <windows.h>
#include "GOST1989.h"

static inline void PrintBytes(std::string const& str)   
{
    std::cout << "[ ";
    for (size_t i = 0; i < str.size(); ++i) std::cout << (uint8_t)str[i] - 0 << " ";
    std::cout << "]" << std::endl;
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
    GOST1989 example{};

    std::string message = GetMessageFromUser();
    std::string key256 = "bkvzhzkyzelebdbyvujamjewljkwomtz";

    std::string messageEncrypt = example.EncryptMessage(message, key256);

    std::string messageDecrypt = example.DecryptMessage(messageEncrypt, key256);

    std::cout << std::endl << "Open message: " << std::endl;
    PrintBytes(message);
    std::cout << message << std::endl << std::endl;

    std::cout << "Encrypted message: " << std::endl;
    PrintBytes(messageEncrypt);
    std::cout << messageEncrypt << std::endl << std::endl;

    std::cout << "Decrypted message: " << std::endl;
    PrintBytes(messageDecrypt);
    std::cout << messageDecrypt << std::endl;
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