#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <regex>

namespace
{
    struct InputData
    {
        uint16_t K = 0;
        uint16_t S = 0;
        uint32_t M = 0;
        uint64_t N = 0;
    };

    struct MatrixData
    {
        uint16_t rowSize;
        uint16_t colSize;
    };

    struct Data
    {
        InputData inputData;
        MatrixData matrixData;
        std::string fullPolynom;
        std::string sequenseOfBits;
    };
}

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

    std::string exeFile = "D:\\WIN32App\\Cryptography\\Lab3.2\\PrimitivePolynomials\\PrimitivePolynomials.exe 2";
    std::wstring fileOutPut = L"D:\\WIN32App\\Cryptography\\Lab3.2\\PrimitivePolynomials\\out.txt";

    std::string cmdToExecute = exeFile.append(" " + std::to_string(M));
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

std::vector<std::string> SearchInStringByRegExp(std::string str, std::regex regExp)
{
    std::vector<std::string> result;

    auto words_begin =
        std::sregex_iterator(str.begin(), str.end(), regExp);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        result.push_back(match_str);
    }

    return result;
}

std::vector<uint32_t> GetAllDegrees(std::vector<std::string> foundsNumbers, std::vector<std::string> foundsX)
{
    std::vector<uint32_t> result;

    for (size_t i = 0; i < foundsNumbers.size(); i++) if (foundsNumbers[i] == "1") foundsNumbers[i] = "0";

    if (!foundsX.empty()) foundsNumbers.push_back("1");

    for (auto const elem : foundsNumbers)
    {
        result.push_back(stoll(elem));
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

std::string GenerateBitSequenseByPolynom(std::string polynom)
{
    std::string result;

    std::regex searchNumber("[0-9]+");
    std::regex searchX("x \\+");

    std::vector<std::string> foundsNumbers = SearchInStringByRegExp(polynom, searchNumber);
    std::vector<std::string> foundsX = SearchInStringByRegExp(polynom, searchX);
    std::vector<uint32_t> allDegrees = GetAllDegrees(foundsNumbers, foundsX);
    
    std::string startSequence;

    for (size_t i = 0; i < allDegrees[0]; i++) startSequence.push_back((i + 1) == allDegrees[0] ? '1' : '0');

    result.append(startSequence);
    std::sort(allDegrees.begin(), allDegrees.end());

    size_t i = 0;
    do
    {
        int num = 0;
        for (size_t j = 0; j < allDegrees.size() - 1; j++) num += result[allDegrees[j] + i] - '0';
        num %= 2;
        result.append(std::to_string(num));
        i++;

    } while (startSequence != result.substr(result.length() - allDegrees[allDegrees.size() - 1], result.length()));

    std::sort(allDegrees.rbegin(), allDegrees.rend());
    result.erase(result.length() - allDegrees[0]);

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
