#include <iostream>
#include <windows.h>
#include "GOSTHash94.h"

std::string GetMessageFromUser()
{
    std::string message;
    std::cout << "Введите сообщение которое хотите хешировать: ";
    std::getline(std::cin, message);
    system("cls");
    return message;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string message = GetMessageFromUser();
    std::string hash = CGOSTHash94::Hashed(message);

    std::cout << "Сообщение: " << message << std::endl;
    std::cout << "Хеш: " << hash << std::endl;
}