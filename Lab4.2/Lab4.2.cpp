#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <windows.h>
#include <random>
#include <map>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using namespace boost::multiprecision;

namespace
{
    struct PointEllipticCurve
    {
        int1024_t x;
        int1024_t y;
    };

    struct SignatureVerification
    {
        int1024_t U1;
        int1024_t U2;
        PointEllipticCurve P;
    };

    struct SignatureAlgorithm
    {
        int1024_t r;
        int1024_t s;
        int1024_t k;
        PointEllipticCurve P;
    };

    struct UserSettings
    {
        std::string message;
        int1024_t hashOfMessage;

        int1024_t Xa;
        PointEllipticCurve Ya;
    };

    struct GeneralSettings
    {
        int1024_t a;
        int1024_t b;
        int1024_t p;

        int1024_t n;
        int1024_t q;
        PointEllipticCurve generator;
    };

    struct Data
    {
        GeneralSettings generalSettings;
        UserSettings userSettings;
        SignatureAlgorithm signatureAlgorithm;
        SignatureVerification DSAVerification;
    };

    bool operator== (const PointEllipticCurve& c1, const PointEllipticCurve& c2)
    {
        return (c1.x == c2.x and c1.y == c2.y);
    }

    std::random_device rd;
    boost::random::mt19937 randomNumber{ rd() };
    boost::random::uniform_int_distribution<> GeneratorInDistribution{ 10000, 120000 };

    std::map<int1024_t, std::vector<std::pair<int1024_t, int1024_t>>> BadABForP;
}

inline int1024_t HashFunc(std::string const& str);
int1024_t inverse_elem(int1024_t const& a, int1024_t const& m);
int1024_t SquareRoot(int1024_t const& e, int1024_t const& p);
bool IsPointAtInfinity(PointEllipticCurve const& result);
std::vector<bool> DecToBin(int1024_t number);
PointEllipticCurve AddingPoints(PointEllipticCurve const& point1, PointEllipticCurve const& point2, GeneralSettings const& genSettings);
PointEllipticCurve DoublingPoint(PointEllipticCurve const& point, GeneralSettings const& genSettings);
PointEllipticCurve Point—omposition(int1024_t const& m, PointEllipticCurve const& point, GeneralSettings const& genSettings);
PointEllipticCurve GetGeneratorPoint(GeneralSettings const& genSettings);
SignatureAlgorithm SignatureAlgorithmFunc(GeneralSettings const& generalSettings, UserSettings const& userSettings);
bool SignatureVerificationFunc(Data& data);
int1024_t GetCountPointOfEliptic—urveFromFile(std::wstring const& filePath);
int1024_t GreatestPrimeDivisorOfNumber(int1024_t n);
int1024_t CountPointsOnElliptic—urve(GeneralSettings& generalSettings);
bool ResistantToMOVAttacks(GeneralSettings const& generalSettings);
UserSettings GetUserSettings(GeneralSettings const& generalSettings);
void PrintInfo(Data const& data);
std::string GetMessageFromUser();
int1024_t modexp(int1024_t const& number, int1024_t const& powerOfnumber, int1024_t const& modulus);
int1024_t CalcD(int1024_t const& a, int1024_t const& b, int1024_t const& p);
void GenerateAB(GeneralSettings& result);
bool IsPrime(int1024_t const& number);
GeneralSettings GetGeneralSettings();
Data StartGOST2001();

inline int1024_t HashFunc(std::string const& str)
{
    int1024_t hash = 0x811c9dc;

    for (size_t i = 0; i < str.size(); ++i)
    {
        uint8_t value = str[i];
        hash ^= value;
    }

    return hash;
}
int1024_t inverse_elem(int1024_t const& a, int1024_t const& m)
{
    for (int1024_t b = 1; ; b++)
    {
        if ((a * b) % m == 1) return b;
    }
}
int1024_t SquareRoot(int1024_t const& e, int1024_t const& p)
{
    for (int1024_t x = 2; x < p; x++)
    {
        if ((x * x) % p == e) return x;
    }
    return -1;
}
bool IsPointAtInfinity(PointEllipticCurve const& result)
{
    return (result.x == 0 and result.y == 0) ? true : false;
}
std::vector<bool> DecToBin(int1024_t number)
{
    std::vector<bool> result;

    while (number > 0)
    {
        result.push_back(static_cast<bool>(number % 2));
        number /= 2;
    }

    return result;
}
PointEllipticCurve AddingPoints(PointEllipticCurve const& point1, PointEllipticCurve const& point2, GeneralSettings const& genSettings)
{
    if (IsPointAtInfinity(point1)) return point2;
    else if (IsPointAtInfinity(point2)) return point1;

    PointEllipticCurve result;
    bool devisor_less_than_zero_flag = false;

    int1024_t devisor = (point2.x - point1.x);

    if (devisor == 0) throw std::logic_error("by zero.");

    while (devisor < 0) devisor = genSettings.p - abs(devisor);

    int1024_t k = (point2.y - point1.y) * inverse_elem(devisor, genSettings.p);

    if (devisor_less_than_zero_flag)
    {
        k *= -1;
        while (k < 0) k = genSettings.p - abs(k);
    }

    result.x = (k * k - point1.x - point2.x) % genSettings.p;
    while (result.x < 0) result.x = genSettings.p - abs(result.x);

    result.y = (k * (point1.x - result.x) - point1.y) % genSettings.p;
    while (result.y < 0) result.y = genSettings.p - abs(result.y);

    return result;
}
PointEllipticCurve DoublingPoint(PointEllipticCurve const& point, GeneralSettings const& genSettings)
{
    if (IsPointAtInfinity(point)) return point;
    PointEllipticCurve result;

    int1024_t k = ((3 * modexp(point.x, 2, genSettings.p)) + genSettings.a) * inverse_elem(2 * point.y, genSettings.p);

    result.x = (k * k - (2 * point.x)) % genSettings.p;
    while (result.x < 0) result.x = genSettings.p - abs(result.x);

    result.y = (k * (point.x - result.x) - point.y) % genSettings.p;
    while (result.y < 0) result.y = genSettings.p - abs(result.y);

    return result;
}
PointEllipticCurve Point—omposition(int1024_t const& m, PointEllipticCurve const& point, GeneralSettings const& genSettings)
{
    PointEllipticCurve result;

    std::vector<bool> binNumber = DecToBin(m);
    std::reverse(binNumber.begin(), binNumber.end());

    result.x = 0;
    result.y = 0;

    for (auto const& bit : binNumber)
    {
        result = DoublingPoint(result, genSettings);
        if (bit)
        {
            result = (point == result) ? DoublingPoint(result, genSettings) 
                : AddingPoints(result, point, genSettings);
        }
    }

    return result;
}
PointEllipticCurve GetGeneratorPoint(GeneralSettings const& genSettings)
{
    do
    {
        PointEllipticCurve result;

        int1024_t x = GeneratorInDistribution(randomNumber) % genSettings.p;
        int1024_t e = (modexp(x, 3, genSettings.p) + genSettings.a * x + genSettings.b) % genSettings.p;
        int1024_t y = SquareRoot(e, genSettings.p);
        if (y == -1) continue;
        else
        {
            result.x = x;
            result.y = y;
        }

        if (genSettings.q == genSettings.n)
        {
            if (!IsPointAtInfinity(result)) return result;
            continue;
        }
        else if (genSettings.q < genSettings.n)
        {
            result = Point—omposition(genSettings.n / genSettings.q, result, genSettings);
            if (!IsPointAtInfinity(result)) return result;
            continue;
        }
        else throw std::logic_error("Error finding generator.");

    } while (true);
}
SignatureAlgorithm SignatureAlgorithmFunc(GeneralSettings const& generalSettings, UserSettings const& userSettings)
{
    SignatureAlgorithm result;

GenerateAgain:
    result.k = GeneratorInDistribution(randomNumber) % generalSettings.p;
    result.P = Point—omposition(result.k, generalSettings.generator, generalSettings);
    result.r = result.P.x % generalSettings.q;
    
    if (result.r == 0) goto GenerateAgain;

    result.s = ((result.k * userSettings.hashOfMessage) + result.r * userSettings.Xa) % generalSettings.q;

    if (result.s == 0) goto GenerateAgain;

    return result;
}
bool SignatureVerificationFunc(Data& data)
{
    if ((data.signatureAlgorithm.r <= 0) or (data.signatureAlgorithm.r >= data.generalSettings.q)
        or (data.signatureAlgorithm.s <= 0) or (data.signatureAlgorithm.s >= data.generalSettings.q))
    {
        return false;
    }
    int1024_t inversH = inverse_elem(data.userSettings.hashOfMessage, data.generalSettings.q);
    data.DSAVerification.U1 = (data.signatureAlgorithm.s * inversH) % data.generalSettings.q;
    data.DSAVerification.U2 = -data.signatureAlgorithm.r * inversH;

    while (data.DSAVerification.U2 < 0) data.DSAVerification.U2 += data.generalSettings.q;

    PointEllipticCurve leftPoint = Point—omposition(data.DSAVerification.U1, data.generalSettings.generator, data.generalSettings);
    PointEllipticCurve rigthPoint = Point—omposition(data.DSAVerification.U2, data.userSettings.Ya, data.generalSettings);

    data.DSAVerification.P = (leftPoint == rigthPoint) ? DoublingPoint(leftPoint, data.generalSettings)
        : AddingPoints(leftPoint, rigthPoint, data.generalSettings);

    if (IsPointAtInfinity(data.DSAVerification.P)) return false;

    return (data.DSAVerification.P.x % data.generalSettings.q == data.signatureAlgorithm.r) ? true : false;
}
std::string GetMessageFromUser()
{
    std::string message;
    std::cout << "Please, enter your message: ";
    std::getline(std::cin, message);
    return message;
}
int1024_t GetCountPointOfEliptic—urveFromFile(std::wstring const& filePath)
{
    int1024_t result = 0;
    std::ifstream file(filePath);
    std::string temp;
    std::getline(file, temp);
    result = stoll(temp);
    file.close();
    return result;
}
int1024_t GreatestPrimeDivisorOfNumber(int1024_t n)
{
    int1024_t currMaxPrime = -1;

    if (n % 2 == 0) 
    {
        currMaxPrime = 2;
        while (n % 2 == 0) n /= 2;
    }

    for (int1024_t i = 3; i <= sqrt(n); i += 2) 
    {
        while (n % i == 0) 
        {
            currMaxPrime = i;
            n /= i;
        }
    }
    if (n > 2) currMaxPrime = n;

    return currMaxPrime;
}
int1024_t CountPointsOnElliptic—urve(GeneralSettings& generalSettings)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    std::string exeFile = "Shoof\\Shoof.exe";
    std::wstring fileOutPut = L"Shoof\\Shoof.txt";

    std::string cmdToExecute = exeFile
        .append(" ")
        .append(std::to_string((uint64_t)generalSettings.p))
        .append(" ")
        .append(std::to_string((uint64_t)generalSettings.a))
        .append(" ")
        .append(std::to_string((uint64_t)generalSettings.b));

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
        TerminateProcess(ProcessInfo.hProcess, 0);
        CloseHandle(ProcessInfo.hThread);
        CloseHandle(ProcessInfo.hProcess);
        generalSettings.n = GetCountPointOfEliptic—urveFromFile(fileOutPut);
        generalSettings.q = GreatestPrimeDivisorOfNumber(generalSettings.n);
    }
    else
    {
        throw std::logic_error("Failed create process.");
    }

    if (generalSettings.n == 0)
    {
        throw std::length_error("Count of points zero.");
    }

    return generalSettings.n;
}
bool ResistantToMOVAttacks(GeneralSettings const& generalSettings)
{
    for (size_t k = 1; k < 32; k++)
    {
        if (modexp(generalSettings.p, k, generalSettings.n) == 1) return false;
    }
    return true;
}
UserSettings GetUserSettings(GeneralSettings const& generalSettings)
{
    UserSettings result;

    result.message = GetMessageFromUser();
    result.hashOfMessage = HashFunc(result.message);
    result.Xa = GeneratorInDistribution(randomNumber) % generalSettings.q;
    result.Ya = Point—omposition(result.Xa, generalSettings.generator, generalSettings);

    return result;
}
void PrintInfo(Data const& data)
{
    system("cls");
    std::cout << "a: " << data.generalSettings.a << std::endl;
    std::cout << "b: " << data.generalSettings.b << std::endl;
    std::cout << "p: " << data.generalSettings.p << std::endl;
    std::cout << "n: " << data.generalSettings.n << std::endl;
    std::cout << "q: " << data.generalSettings.q << std::endl;
    std::cout << "Generator: (" << data.generalSettings.generator.x << ", " << data.generalSettings.generator.y <<")" << std::endl << std::endl;

    std::cout << "message: " << data.userSettings.message << std::endl;
    std::cout << "hash: " << data.userSettings.hashOfMessage << std::endl;
    std::cout << "Xa: " << data.userSettings.Xa << std::endl;
    std::cout << "Ya: (" << data.userSettings.Ya.x << ", " << data.userSettings.Ya.y << ")" << std::endl << std::endl;

    std::cout << "k: " << data.signatureAlgorithm.k << std::endl;
    std::cout << "P: (" << data.signatureAlgorithm.P.x << ", " << data.signatureAlgorithm.P.y << ")" << std::endl;
    std::cout << "r: " << data.signatureAlgorithm.r << std::endl;
    std::cout << "s: " << data.signatureAlgorithm.s << std::endl << std::endl;;

    std::cout << "U1: " << data.DSAVerification.U1 << std::endl;
    std::cout << "U2: " << data.DSAVerification.U2 << std::endl;
    std::cout << "P: (" << data.DSAVerification.P.x << ", " << data.DSAVerification.P.y << ")" << std::endl << std::endl;

    std::cout << "x: " << data.DSAVerification.P.x % data.generalSettings.q << "r: " << data.signatureAlgorithm.r << std::endl;
}
int1024_t modexp(int1024_t const& number, int1024_t const& powerOfnumber, int1024_t const& modulus)
{
    if (powerOfnumber == 0) return 1;
    int1024_t z = modexp(number, powerOfnumber / 2, modulus);
    if (powerOfnumber % 2 == 0)
        return (z * z) % modulus;
    else
        return (number * z * z) % modulus;
}
int1024_t CalcD(int1024_t const& a, int1024_t const& b, int1024_t const& p)
{
    return (4 * modexp(a, 3, p) + 27 * modexp(b, 2, p)) % p;
}
void GenerateAB(GeneralSettings& result)
{
    do
    {
    GenerateAgain:
        result.a = GeneratorInDistribution(randomNumber);
        result.b = GeneratorInDistribution(randomNumber);

        if (BadABForP.find(result.p) != BadABForP.end())
        {
            for (auto const& pair : BadABForP.find(result.p)->second)
            {
                if (pair == std::make_pair(result.a, result.b)) goto GenerateAgain;
            }
        }
    } while (
        CalcD(result.a, result.b, result.p) == 0
        or (result.p <= result.a)
        or (result.p <= result.b)
        or (result.a % result.p == 0)
        or (result.b % result.p == 0));
}
bool IsPrime(int1024_t const& number)
{
    for (int1024_t i = 2; i <= sqrt(number); i++)
    {
        if (number % i == 0) return false;
    }
    return true;
}
GeneralSettings GetGeneralSettings()
{
    GeneralSettings generalSettings;

GenerateAgain:
    do generalSettings.p = GeneratorInDistribution(randomNumber);
    while (!IsPrime(generalSettings.p));

    GenerateAB(generalSettings);

    if (!IsPrime(CountPointsOnElliptic—urve(generalSettings))
        or !ResistantToMOVAttacks(generalSettings)
        or generalSettings.p == generalSettings.q)
    {
        BadABForP[generalSettings.p].push_back(std::make_pair(
            generalSettings.a, generalSettings.b)
        );
        goto GenerateAgain;
    }

    std::cout << "a: " << generalSettings.a << std::endl;
    std::cout << "b: " << generalSettings.b << std::endl;
    std::cout << "p: " << generalSettings.p << std::endl;
    std::cout << "n: " << generalSettings.n << std::endl;
    std::cout << "q: " << generalSettings.q << std::endl << std::endl;

    generalSettings.generator = GetGeneratorPoint(generalSettings);

    return generalSettings;
}
Data StartGOST2001()
{
    Data GOSTData;

    // I. √≈Õ≈–¿÷»ﬂ a b p, (Ì‡ıÓÊ‰ÂÌËÂ ÍÓÎÎË˜ÂÒÚ‚Ó ÚÓ˜ÂÍ n) Ë (·OÎ¸¯ËÈ ÔÓÒÚÓÈ ‰ÂÎËÚÂÎ¸ ˜ËÒÎ‡ n = q) => ÔÓËÒÍ „ÂÌÂ‡ÚÓ‡ 
    GOSTData.generalSettings = GetGeneralSettings();

    // II. ¬¬Œƒ œŒÀ‹«Œ¬¿“≈À≈Ã —ŒŒ¡Ÿ≈Õ»ﬂ » ¬€¡Œ– k
    GOSTData.userSettings = GetUserSettings(GOSTData.generalSettings);

    // III. ¿À√Œ–»“Ã œŒƒœ»—»
    GOSTData.signatureAlgorithm = SignatureAlgorithmFunc(GOSTData.generalSettings, GOSTData.userSettings);

    // III. ¿À√Œ–»“Ã œŒƒœ»—»
    if (SignatureVerificationFunc(GOSTData))
    {
        std::cout << "œÓ‰ÔËÒ¸ ‚ÂÌ‡." << std::endl;
    }
    else
    {
        std::cout << "œÓ‰ÔËÒ¸ ÌÂ ‚ÂÌ‡." << std::endl;
    }

    return GOSTData;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

again:
    try
    {
        Data GOSTdata = StartGOST2001();
        PrintInfo(GOSTdata);
    }
    catch (const std::exception&)
    {
        goto again;
    }
}