#include <iostream>
#include <windows.h>
#include "SHA256.h"

std::string GetMessageFromUser()
{
    std::string message;
    std::cout << "������� ��������� ������� ������ ����������: ";
    std::getline(std::cin, message);
    system("cls");
    return message;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string message = GetMessageFromUser();
    std::string hash = SHA256::Hashed(message);

    std::cout << "���������: " << message << std::endl;
    std::cout << "���: " << hash << std::endl;
}