#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

struct InputData
{
    uint16_t K = 0;
    uint16_t S = 0;
    uint32_t M = 0;
    uint64_t N = 0;
};

struct Data
{
    InputData inputData;
    std::string fullPolynom;
    std::vector<std::string> AllFullPolynom;
    std::string sequenseOfBits;
};

void CheckAgruments(int argc)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << std::endl
            << "\tLab3.2.exe <K> <S>" << std::endl
            << "\t\t<K> - k should be >= 2." << std::endl
            << "\t\t<S> - s should be >= 3." << std::endl;

        throw std::invalid_argument("Invalid arguments count.");
    }
}
void ValidateArguments(int argc, char* argv[])
{
    CheckAgruments(argc);

	if (std::stoi(argv[1]) < 2)
	{
		throw std::invalid_argument("Error K.");
	}
    else if (std::stoi(argv[2]) < 3)
    {
        throw std::invalid_argument("Error S.");
    }
}

std::string GetfullPolynomFromFile(std::wstring filePath)
{
    std::ifstream file(filePath);
    std::string str((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    return str;
}
std::string GeneratePrimitivePolynomial(uint64_t M)
{
    std::string result = "";
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    std::string exeFile = "D:\\WIN32App\\Cryptography\\Lab3.2\\PrimitivePolynomials\\PrimitivePolynomials.exe";
    std::wstring fileOutPut = L"D:\\WIN32App\\Cryptography\\Lab3.2\\PrimitivePolynomials\\out.txt";

    std::string cmdToExecute = exeFile.append(" " + std::to_string(2)).append(" " + std::to_string(M));
    std::wstring cmdToExecuteW = std::wstring(cmdToExecute.begin(), cmdToExecute.end());

    HANDLE hFile = CreateFile(fileOutPut.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION ProcessInfo;
    STARTUPINFO StartupInfo;
    BOOL ret = FALSE;
    DWORD flags = CREATE_NO_WINDOW;
    ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));

    StartupInfo.cb = sizeof(STARTUPINFO);
    StartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    StartupInfo.hStdInput = NULL;
    StartupInfo.hStdError = hFile;
    StartupInfo.hStdOutput = hFile;

    if (CreateProcess(
        NULL,
        &cmdToExecuteW[0],
        NULL,
        NULL,
        TRUE,
        flags,
        NULL,
        NULL,
        &StartupInfo,
        &ProcessInfo))
    {
        WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);

        result = GetfullPolynomFromFile(fileOutPut);
    }
    else
    {
        throw std::logic_error("Failed create process.");
    }

    if (result.empty())
    {
        throw std::length_error("Polynom is empty.");
    }

    return result;
}
std::string GenerateBitSequenseByPolynom(std::string polynom)
{
    std::string result;
    


    return result;
}

void StartGMW(char* argv[])
{
	Data GMWData;
    GMWData.inputData.K = std::stoll(argv[1]);
    GMWData.inputData.S = std::stoll (argv[2]);
    GMWData.inputData.M = GMWData.inputData.S * GMWData.inputData.K;
    GMWData.inputData.N = pow(2, GMWData.inputData.M) - 1;
    GMWData.fullPolynom = GeneratePrimitivePolynomial(GMWData.inputData.M);
    GMWData.sequenseOfBits = GenerateBitSequenseByPolynom(GMWData.fullPolynom);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, ".1251");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	std::cout << "GMW-sequence." << std::endl << std::endl;

	try
	{
		ValidateArguments(argc, argv);
		StartGMW(argv);
	}
	catch (std::exception & error)
	{
		std::cout << error.what() << std::endl;
		return 1;
	}
}
