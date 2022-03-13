#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <fstream>
#include <regex>

namespace
{
    struct InputData
    {
        uint64_t K = 0;
        uint64_t S = 0;
        uint64_t M = 0;
        uint64_t N = 0;
    };
    
    struct PolynomMirorRowSequence
    {
        std::string polynomByPowerS;
        std::string rowBitSequens;
    };

    struct PolynomData
    {
        std::vector <std::string> polynomsByPowerM;
        std::vector <std::string> polynomsByPowerS;
        std::vector <PolynomMirorRowSequence> vectorPolynomsByPowerS;
        PolynomMirorRowSequence selectedPolynomS;
        std::string selectedPolynomM;
    };

    struct MatrixData
    {
        uint64_t rowSize = 0;
        uint64_t colSize = 0;
        std::vector<std::vector<uint64_t>> matrixA;
        std::vector<std::vector<uint64_t>> matrixB;
        std::vector<uint64_t> vectorOfRotate;
        std::vector<uint64_t> firstRowNonZeros;
    };

    struct Data
    {
        InputData inputData;
        MatrixData matrixData;
        PolynomData polynomData;
        std::string sequenseOfBits;
        std::string GMWSequence;
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

std::vector<std::string> GetAllPolynomialsFromFile(std::wstring filePath)
{
    std::vector<std::string> result;
    std::ifstream file(filePath);
    std::string temp;
    while (getline(file, temp)) result.push_back(temp);
    file.close();
    return result;
}

std::vector<std::string> GeneratePrimitivePolynomials(uint64_t degree)
{
    std::vector<std::string> result;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;                                                                                                                                                               

    std::string exeFile = "PrimitivePolynomials\\PrimitivePolynomials.exe -a 2";
    std::wstring fileOutPut = L"PrimitivePolynomials\\AllPrimitivePolynomials.txt";

    std::string cmdToExecute = exeFile.append(" " + std::to_string(degree));
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
        WaitForSingleObject(ProcessInfo.hProcess, 5000);
        TerminateProcess(ProcessInfo.hProcess, 0);
        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);
        result = GetAllPolynomialsFromFile(fileOutPut);
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

std::vector<uint64_t> GetAllDegrees(std::string polynomByPower)
{
    std::regex searchNumber("[0-9]+");
    std::regex searchX("x \\+");

    std::vector<std::string> foundsNumbers = SearchInStringByRegExp(polynomByPower, searchNumber);
    std::vector<std::string> foundsX = SearchInStringByRegExp(polynomByPower, searchX);

    std::vector<uint64_t> result;

    for (size_t i = 0; i < foundsNumbers.size(); i++) if (foundsNumbers[i] == "1") foundsNumbers[i] = "0";

    if (!foundsX.empty()) foundsNumbers.push_back("1");

    for (auto const& elem : foundsNumbers)
    {
        result.push_back(static_cast<uint64_t>(stoul(elem)));
    }

    std::sort(result.rbegin(), result.rend());

    return result;
}

std::string GenerateBitSequencse(std::vector<uint64_t> allDegrees, std::string startSequence)
{
    std::string result;
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

std::string GenerateBitSequenseByPolynomM(std::string polynomByPowerM)
{
    std::vector<uint64_t> allDegrees = GetAllDegrees(polynomByPowerM);
    std::string startSequence;

    for (size_t i = 0; i < allDegrees[0]; i++) startSequence.push_back((i + 1) == allDegrees[0] ? '1' : '0');

    std::string result = GenerateBitSequencse(allDegrees, startSequence);

    return result;
}

std::vector<std::string> CutOfBitSequence(std::string bitSequence, uint64_t row)
{
    std::vector<std::string> result;

    for (size_t i = 0; i < bitSequence.size(); i += row)
    {
        result.push_back(bitSequence.substr(i, row));
    }

    return result;
}

std::vector<std::vector<uint64_t>> FillMatrixFromBinSequence(uint64_t col, uint64_t row, std::string bitSequence)
{
    std::vector<std::vector<uint64_t>> result;
    std::vector<std::string> vectorOfRowBits = CutOfBitSequence(bitSequence, row);

    for (size_t i = 0; i < row; i++)
    {
        std::vector<uint64_t> matrixRow;
        for (size_t j = 0; j < col; j++)
        {
            matrixRow.push_back(vectorOfRowBits[j][i] - '0');
        }
        result.push_back(matrixRow);
    }

    return result;
}

std::string SelectPolynomFromList(std::vector<std::string> polynomList)
{
    std::string polynom;

    std::cout << "Select polynom by power W: " << std::endl << std::endl;

    for (size_t i = 0; i < polynomList.size(); i++)
    {
        std::cout << i << ": " << polynomList[i] << std::endl;
    }

    int select;
    do
    {
        std::cin >> select;
    } while (select >= polynomList.size() or select < 0);

    return polynomList[select];
}

PolynomMirorRowSequence SelectPolynomFromList(std::vector <PolynomMirorRowSequence> polynomList)
{
    PolynomMirorRowSequence polynom;

    std::cout << "Select polynom by power S: " << std::endl << std::endl;

    for (size_t i = 0; i < polynomList.size(); i++)
    {
        std::cout << i << ": " << polynomList[i].polynomByPowerS 
            << ", " << polynomList[i].rowBitSequens << std::endl;
    }

    int select;
    do
    {
        std::cin >> select;
    } while (select >= polynomList.size() or select < 0);

    return polynomList[select];
}

std::vector<uint64_t> GetFirstRowNonZerosFromMatrix(std::vector<std::vector<uint64_t>> matrix)
{
    std::vector<uint64_t> result;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (std::all_of(matrix[i].begin(), matrix[i].end(), [](uint64_t i) {return i == 0; })) continue;
        return matrix[i];
    }
    throw std::invalid_argument("Non no zeros row.");
}

std::vector<uint64_t> GetVectorRotateFromMatrix(std::vector<std::vector<uint64_t>> matrix, std::vector<uint64_t> firstRowNonZeros)
{
    std::vector<uint64_t> result;
    
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (std::all_of(matrix[i].begin(), matrix[i].end(), [](uint64_t i) {return i == 0; }))
        {
            result.push_back(-1);
            continue; //skip all zeros row from start
        }

        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            std::vector<uint64_t> temp = matrix[i];
            std::rotate(temp.begin(), temp.begin() + j, temp.end());
            if (temp == firstRowNonZeros)
            {
                result.push_back(static_cast<uint64_t>(j));
                break;
            }
        }
    }

    return result;
}

std::vector<uint64_t> StringToVector(std::string bitSequence)
{
    std::vector<uint64_t> result;
    for (size_t i = 0; i < bitSequence.size(); i++) result.push_back(bitSequence[i] - '0');
    return result;
}

std::string VectorToString(std::vector<uint64_t> vect)
{
    std::string result;
    for (size_t i = 0; i < vect.size(); i++) result.append(std::to_string(vect[i]));
    return result;
}

std::vector <PolynomMirorRowSequence> GetMappingRowSequenceByPolynom(std::vector <std::string> polynomsByPowerS, std::vector<uint64_t> firstRowNonZeros, uint64_t S)
{
    std::vector <PolynomMirorRowSequence> result;

    std::string startSequence;
    for (size_t i = 0; i < S; i++) startSequence.append(std::to_string(firstRowNonZeros[i]));

    for (size_t i = 0; i < polynomsByPowerS.size(); i++)
    {
        PolynomMirorRowSequence temp;
        temp.polynomByPowerS = polynomsByPowerS[i];
        std::vector<uint64_t> bitSequenceByPowerS = StringToVector(GenerateBitSequencse(GetAllDegrees(polynomsByPowerS[i]), startSequence));

        if (bitSequenceByPowerS == firstRowNonZeros) continue;

        temp.rowBitSequens = GenerateBitSequencse(GetAllDegrees(polynomsByPowerS[i]), startSequence);

        result.push_back(temp);
    }
    return result;
}

std::vector<std::vector<uint64_t>> GenerateNewMatrixBySelectedPolynomS(std::vector<uint64_t> vectorOfRotate, PolynomMirorRowSequence selectedPolynomS, uint64_t col, uint64_t row)
{
    std::vector<std::vector<uint64_t>> result;

    std::vector<uint64_t> sequenceToRotate = StringToVector(selectedPolynomS.rowBitSequens);
    std::vector<uint64_t> vectorOfZeros(col, 0);
    
    for (size_t i = 0; i < vectorOfRotate.size(); i++)
    {
        if (vectorOfRotate[i] == 18446744073709551615)
        {
            result.push_back(vectorOfZeros);
            continue;
        }
        std::vector<uint64_t> temp = sequenceToRotate;
        std::rotate(temp.rbegin(), temp.rbegin() + vectorOfRotate[i], temp.rend());
        result.push_back(temp);
    }

    return result;
}

std::string GetGMWSequenceFromMatrixB(std::vector<std::vector<uint64_t>> matrixB)
{
    std::string result;

    for (size_t i = 0; i < matrixB[0].size(); i++)
    {
        for (size_t j = 0; j < matrixB.size(); j++)
        {
            result.append(std::to_string(matrixB[j][i]));
        }
    }
    return result;
}

Data StartGMW(char* argv[])
{
	Data GMWData;
    GMWData.inputData.K = static_cast<uint64_t>(std::stoull(argv[1]));
    GMWData.inputData.S = static_cast<uint64_t>(std::stoull(argv[2]));
    GMWData.inputData.M = GMWData.inputData.S * GMWData.inputData.K;
    GMWData.inputData.N = static_cast<uint64_t>(pow(2, GMWData.inputData.M) - 1);

    GMWData.polynomData.selectedPolynomM = SelectPolynomFromList(GeneratePrimitivePolynomials(GMWData.inputData.M));

    GMWData.sequenseOfBits = GenerateBitSequenseByPolynomM(GMWData.polynomData.selectedPolynomM);

    GMWData.matrixData.colSize = static_cast<uint64_t>(pow(2, GMWData.inputData.S) - 1);
    GMWData.matrixData.rowSize = static_cast<uint64_t>(GMWData.inputData.N / GMWData.matrixData.colSize);
    GMWData.matrixData.matrixA = FillMatrixFromBinSequence(GMWData.matrixData.colSize, GMWData.matrixData.rowSize, GMWData.sequenseOfBits);
    GMWData.matrixData.firstRowNonZeros = GetFirstRowNonZerosFromMatrix(GMWData.matrixData.matrixA);
    GMWData.matrixData.vectorOfRotate = GetVectorRotateFromMatrix(GMWData.matrixData.matrixA, GMWData.matrixData.firstRowNonZeros);

    GMWData.polynomData.polynomsByPowerS = GeneratePrimitivePolynomials(GMWData.inputData.S);
    GMWData.polynomData.selectedPolynomS = SelectPolynomFromList(GetMappingRowSequenceByPolynom(GMWData.polynomData.polynomsByPowerS, GMWData.matrixData.firstRowNonZeros, GMWData.inputData.S));

    GMWData.matrixData.matrixB = GenerateNewMatrixBySelectedPolynomS(GMWData.matrixData.vectorOfRotate, GMWData.polynomData.selectedPolynomS, GMWData.matrixData.colSize, GMWData.matrixData.rowSize);
    GMWData.GMWSequence = GetGMWSequenceFromMatrixB(GMWData.matrixData.matrixB);

    return GMWData;
}

void PrintGMWInfo(Data const& data)
{
    system("cls");

    std::cout << "GMW-sequence." << std::endl << std::endl;

    std::cout << "K: " << data.inputData.K << std::endl << std::endl;
    std::cout << "S: " << data.inputData.S << std::endl << std::endl;
    std::cout << "M: " << data.inputData.M << std::endl << std::endl;
    std::cout << "N: " << data.inputData.N << std::endl << std::endl;

    std::cout << "Polynom M: " << data.polynomData.selectedPolynomM << std::endl << std::endl;
    std::cout << "Generated Sequence: " << data.sequenseOfBits << std::endl << std::endl;

    std::cout << "Matrix colum: " << data.matrixData.colSize << std::endl << std::endl;
    std::cout << "Matrix row: " << data.matrixData.rowSize << std::endl << std::endl;
    std::cout << "Matrix sequence to rotate: " << VectorToString(data.matrixData.firstRowNonZeros) << std::endl << std::endl;
    std::cout << "Vector of rotation (size): " << data.matrixData.vectorOfRotate.size() << std::endl << std::endl;

    std::cout << "Polynom S: " << data.polynomData.selectedPolynomS.polynomByPowerS << std::endl << std::endl;
    std::cout << "Start sequence of polynom S: " << data.polynomData.selectedPolynomS.rowBitSequens << std::endl << std::endl;

    std::cout << "GMW sequence: " << data.GMWSequence << std::endl << std::endl;
}

void PrintGMWSequenceToFile(std::string const& GMW)
{
    std::ofstream file("PrimitivePolynomials\\GMW.txt");
    if (file.is_open()) file << GMW;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, ".1251");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	try
	{
		ValidateArguments(argc, argv);
        Data GMWdata = StartGMW(argv);
        PrintGMWInfo(GMWdata);
        PrintGMWSequenceToFile(GMWdata.GMWSequence);
	}
	catch (std::exception & error)
	{
		std::cout << error.what() << std::endl;
		return 1;
	}
}
