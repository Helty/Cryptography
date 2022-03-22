#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using namespace std;
using namespace boost::multiprecision;

namespace
{
    struct SignatureVerification
    {
        uint256_t U1;
        uint256_t U2;
        uint256_t V;
    };

    struct SignatureAlgorithm
    {
        uint256_t k;
        uint256_t r;
        uint256_t s;
    };

    struct UserSettings
    {
        uint256_t x;
        uint256_t y;
        int message = 0;
        uint256_t hash;
    };

    struct GeneralSettings
    {
        uint256_t q;
        uint1024_t p;
        uint256_t a;
    };

    struct Data
    {
        GeneralSettings generalSettings;
        UserSettings userSettings;
        SignatureAlgorithm signatureAlgorithm;
        SignatureVerification DSAVerification;
    };

    boost::random::mt19937 gen { static_cast<uint32_t>(std::time(0)) };
    boost::random::uniform_int_distribution<uint256_t> Gen256{ 5000000, 10000000 };
}

bool IsPrime(uint256_t number)
{
    for (auto a = 2; a * a < number; a++)
    {
        if (number % a == 0) return false;
    }
    return true;
}
bool IsPrime(uint1024_t number)
{
    for (auto a = 2; a * a < number; a++)
    {
        if (number % a == 0) return false;
    }
    return true;
}
uint1024_t modexp(uint256_t number, uint256_t powerOfnumber, uint1024_t modulus)
{
    if (powerOfnumber == 0) return 1;
    uint1024_t z = modexp(number, powerOfnumber / 2, modulus);
    if (powerOfnumber % 2 == 0)
        return (z * z) % modulus;
    else
        return (number * z * z) % modulus;
}
uint256_t HashFunc(int message)
{
    return message + 3;
}

uint256_t generate_r(uint256_t a, uint256_t k, uint1024_t p, uint256_t q)
{
    uint256_t r = 0;
    while (r == 0) r = static_cast<uint256_t>(modexp(a, k, p) % q);
    return r;
}
uint256_t generate_k(uint256_t q)
{
    return Gen256(gen) % q;
}
uint256_t generate_x(uint256_t q)
{
    return Gen256(gen) % q;
}
uint256_t generate_a(uint1024_t p, uint256_t q) 
{
    for (uint256_t a = 2; ; a++)
    {
        if (modexp(a, q, p) == 1) return a;
    }
}
uint1024_t generate_p(uint256_t q)
{
    uint1024_t p;
    for (uint256_t B = 1; ; B++)
    {
        p = B * q + 1;
        if (IsPrime(p)) return p;
    }
}
uint256_t generate_q() 
{
    uint256_t q;
    do
    {
        q = Gen256(gen);

    } while (!IsPrime(q));
    return q;
}

uint256_t inverse_elem(uint256_t a, uint256_t m)
{
    /*обратный эелемент
    Обратным к числу a по модулю m называется такое число b, что:
    ab = 1 (mod m)
    */
    for (auto b = 1; ; b++) if ((a * b) % m == 1) return b;
}
uint256_t find_V(uint256_t U1, uint256_t U2, uint256_t a, uint256_t y, uint1024_t p, uint256_t q)
{
    // V = (a^U1) * (y^U2) (mod p) (mod q)
    uint1024_t V, tmp1, tmp2, tmp3;
    tmp1 = modexp(a, U1, p);
    tmp2 = modexp(y, U2, p);
    tmp3 = (tmp1 * tmp2) % p;
    V = tmp3 % q;
    return static_cast<uint256_t>(V);
}
uint256_t find_s(uint256_t k, uint256_t h, uint256_t x, uint256_t r, uint256_t q)
{
    return static_cast<uint256_t>((k * h + x * r) % q);
}
uint256_t find_U(uint256_t s_r, uint256_t h0, uint256_t q, bool minus = false)
{
    uint256_t h = inverse_elem(h0, q); // обратный эелемент h (mod q)
    uint256_t U, s_r0;
    if (minus)
    {
        s_r0 = q - abs(s_r);
        U = (s_r0 * h) % q;
    }
    else U = (s_r * h) % q;
    return U;
}

GeneralSettings PQAGenerate()
{
    GeneralSettings result;

    result.q = generate_q();
    result.p = generate_p(result.q);
    result.a = generate_a(result.p, result.q);

    return result;
}
UserSettings XYAGenerate(GeneralSettings const& genSettings)
{
    UserSettings result;

    result.x = generate_x(genSettings.q);
    result.y = static_cast<uint256_t>(modexp(genSettings.a, result.x, genSettings.p));

    return result;
}
SignatureAlgorithm SignatureAlgorithmFunc(GeneralSettings const& generalSettings, UserSettings const& userSettings, uint256_t hash)
{
    SignatureAlgorithm outputDSA;

    do
    {
        outputDSA.k = generate_k(generalSettings.q);
        outputDSA.r = generate_r(generalSettings.a, outputDSA.k, generalSettings.p, generalSettings.q);
        outputDSA.s = find_s(outputDSA.k, hash, userSettings.x, outputDSA.r, generalSettings.q);

    } while (outputDSA.r == 0 or outputDSA.s == 0);

    return outputDSA;
}

SignatureVerification SignatureVerificationFunc(
    GeneralSettings const& generalSettings, 
    UserSettings const& userSettings, 
    SignatureAlgorithm const& signatureAlgorithm
)
{
    SignatureVerification result;

    result.U1 = find_U(signatureAlgorithm.s, userSettings.hash, generalSettings.q);
    result.U2 = find_U(signatureAlgorithm.r, userSettings.hash, generalSettings.q, true);
    result.V = find_V(result.U1, result.U2, generalSettings.a, userSettings.y, generalSettings.p, generalSettings.q);
    cout << "V должно быть равно r : V = " << result.V << ", r = " << signatureAlgorithm.r << endl;

    return result;
}

Data StartGOST94()
{
    // ДАННЫЕ
    Data GOSTData;

    // I
    GOSTData.generalSettings = PQAGenerate();

    // II
    GOSTData.userSettings = XYAGenerate(GOSTData.generalSettings);
    GOSTData.userSettings.message = 10;
    GOSTData.userSettings.hash = HashFunc(GOSTData.userSettings.message);

    // III АЛГОРИТМ ПОДПИСИ
    GOSTData.signatureAlgorithm = SignatureAlgorithmFunc(GOSTData.generalSettings, GOSTData.userSettings, GOSTData.userSettings.hash);

    //АЛГОРИТМ ПРОВЕРКИ
    GOSTData.DSAVerification = SignatureVerificationFunc(GOSTData.generalSettings, GOSTData.userSettings, GOSTData.signatureAlgorithm);

    return GOSTData;
}

int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    /*
    I. ОБЩИЕ НАСТРОЙКИ p q a
    1.выбор q - простое число-> функц генерир простое число	 
    2. p = B*q+1 - тоже простое число должно быть. -> подбираем такое B
    3. найти a^q (mod p) == 1 -> подбираем такое a

    II. ПОЛЬЗОВАТ НАСТРОЙКИ 
    1. x- секр ключ такой, что 0 < x < q
    2. y - отк ключ y=a^x (mod p)

    III. АЛГОРИТМ ПОДПИСИ
    1. вычисля хэш-функ h=H(m), где m - наше сообщение, при этом  0< h < q; h=m+(3)
    2. генерируем некое раовое число k такео, что 0<k<q
    3. генерируем подпись r=a^k (mod p) (mod q), если r==0 , то обратно на шаг 2 генерир к пока r!=0
    4. вычисляем s=(k*h + x*r) (mod q) , при этом s != 0 иначе шаг 2 и генер к

    АЛГОРИТМ ПРОВЕРКИ
    1. хэш-функ h=H(m) h=m+(3)
    2. вычисляем вспомогат параметры U1=s*h^(-1) (mod q)
       U2 = -r *h^(-1) (mod p) (mod q)
    3. V = (a^U1) * (y^U2) (mod p) (mod q)
    4.  если r = V , то подпись верна и текст можно читать
    */

    try
    {
        for (size_t i = 0; i < 50; i++)
        {
            Data GOSTdata = StartGOST94();
        }
    }
    catch (const std::exception& err)
    {
        cout << err.what() << endl;
        return 1;
    }
}