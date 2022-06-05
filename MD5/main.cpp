#include <iostream>
#include <windows.h>
#include "MD5.h"

std::string GetMessageFromUser()
{
    std::string message;
    std::cout << "Input message to hashed: ";
    std::getline(std::cin, message);
    system("cls");
    return message;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string message = GetMessageFromUser();
    std::string hash = CMD5::Hashed(message);

    std::cout << "Message: " << message << std::endl;
    std::cout << "Hash: " << hash << std::endl;
}