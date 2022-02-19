#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;

namespace
{
	struct PrimalPair
	{
		uint32_t p1;
		uint32_t p2;
	};
	struct PublicKey
	{
		uint32_t n;
		uint32_t e;
	};
	struct PrivateKey
	{
		uint32_t n;
		uint32_t d;
	};
	struct RSA
	{
		PublicKey publicKey;
		PrivateKey privateKey;
	};
	using ArrayPrimal = vector<uint32_t>;
}

string readFile(const char*);
void writeInFile(string const&, const char*);
ArrayPrimal getPrimalsNumberFromFile(const char*);
PrimalPair getPairPrimalNumber(ArrayPrimal&);
uint32_t calcPublicExponent(uint32_t);
uint32_t calcModule(PrimalPair);
uint32_t calcEuler(PrimalPair);
uint32_t calcSecretExponent(uint32_t, uint32_t);
uint32_t greatestCommonDivisor(uint32_t, uint32_t);
bool isRelativelySimple(uint32_t, uint32_t);
uint32_t GCD(uint32_t a, uint32_t b);
string encryptText(string const&, PublicKey);
string encrypt(byte, PublicKey);
string decryptText(string const&, PrivateKey);
byte decrypt(string, PrivateKey);

//работа с файлами
string readFile(const char* filename)
{
	ifstream fileIn(filename, std::ios::binary);
	string str((std::istreambuf_iterator<char>(fileIn)),
		std::istreambuf_iterator<char>());
	fileIn.close();
	return str;
}
void writeInFile(string const& encryptText, const char* filename)
{
	std::ofstream file(filename);
	file << encryptText;
	file.close();
}

//подсёт p q n Ф(n) e d
ArrayPrimal getPrimalsNumberFromFile(const char* primalsFile)
{
	ArrayPrimal result;
	string primals = readFile(primalsFile);
	istringstream iss(primals);
	string item;
	while (getline(iss, item, ' '))
	{
		result.push_back(stoi(item));
	}
	return result;
}
PrimalPair getPairPrimalNumber(ArrayPrimal& primals)
{
	if (primals.empty())
	{
		throw invalid_argument("Primals numbers is out.");
	}

	PrimalPair result;

	result.p1 = primals.back();
	primals.pop_back();
	result.p2 = primals.back();
	primals.pop_back();

	return result;
}
uint32_t calcModule(PrimalPair primalPair)
{
	return (primalPair.p1 * primalPair.p2);
}
uint32_t calcEuler(PrimalPair primalPair)
{
	return ((primalPair.p1 - 1) * (primalPair.p2 - 1));
}
uint32_t calcSecretExponent(uint32_t e, uint32_t euler)
{
	uint32_t d;
	uint32_t k = 1;

	while (true)
	{
		k = k + euler;

		if (k % e == 0)
		{
			d = (k / e);
			return d;
		}
	}

	throw logic_error("Non secret exponent");
}
uint32_t calcPublicExponent(uint32_t euler)
{
	uint32_t e;

	for (e = 2; e < euler; e++)
	{
		if (greatestCommonDivisor(e, euler) == 1) return e;
	}

	throw logic_error("Non public exponent");
}

//вспомогательные функции
uint32_t greatestCommonDivisor(uint32_t e, uint32_t euler)
{
	while (e > 0)
	{
		uint32_t myTemp;

		myTemp = e;
		e = euler % e;
		euler = myTemp;
	}

	return euler;
}
bool isRelativelySimple(uint32_t primal, uint32_t euler)
{
	return (GCD(primal, euler) == 1) ? true : false;
}
uint32_t GCD(uint32_t a, uint32_t b)
{
	return b ? GCD(b, a % b) : a;
}
uint32_t modPov(uint32_t base, uint32_t pov, uint32_t modul)
{
	uint32_t res = 1;
	base = base % modul;
	if (base == 0) return 0;
	while (pov > 0)
	{
		if (pov & 1)
		{
			res = (res * base) % modul;
		}
		pov = pov >> 1;
		base = (base * base) % modul;
	}
	return res;
}

//шифрование/дешифрование
string encryptText(string const& text, PublicKey key)
{
	string result;
	for (size_t i = 0; i < text.length(); i++)
	{
		result += encrypt(text[i], key);
	}
	return result;
}
string encrypt(byte code, PublicKey key)
{
	uint32_t result = 1;
	for (size_t i = 0; i < key.e; i++)
	{
		result *= code;
		result %= key.n;
	}
	string res = to_string(result);
	if (!(res.size() == to_string(key.n).size()))
	{
		size_t count = to_string(key.n).size() - res.size();
		for (size_t i = 0; i < count; i++)
		{
			res.insert(0, "0");
		}
	}
	return res;
}
string decryptText(string const& text, PrivateKey key)
{
	string result;
	uint32_t start = 0;
	uint32_t step = to_string(key.n).length();
	uint32_t encryptTextSize = (text.length() / step);
	for (size_t i = 0; i < encryptTextSize; i++)
	{
		result += decrypt(text.substr(start, step), key);
		start += step;
	}
	return result;
}
byte decrypt(string code, PrivateKey key)
{
	while (code[0] == '0') code.erase(0, 1);
	uint32_t codeInt = stoi(code);
	uint32_t result = modPov(codeInt, key.d, key.n);
	return result;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		if (argc != 5)
		{
			cout << "Usage: " << endl
				<< "\tcrypt.exe <file primal> <file text> <file encryptText> <file decryptText>" << endl
				<< "\t\t<file primal> - path to file, where stored primal numbers." << endl
				<< "\t\t<file text> - path to file, where stored text to encrypt." << endl
				<< "\t\t<file encryptText> - path to file, where will be encryption text." << endl
				<< "\t\t<file decryptText> - path to file, where will be decryption text." << endl;
			throw invalid_argument("Invalid arguments count");
		}

		ArrayPrimal primals = getPrimalsNumberFromFile(argv[1]);
		PrimalPair primalPair = getPairPrimalNumber(primals);
		uint32_t n = calcModule(primalPair);
		uint32_t Euler = calcEuler(primalPair);
		uint32_t e = calcPublicExponent(Euler);
		uint32_t d = calcSecretExponent(e, Euler);
		RSA rsaKeys { /*public key*/{ n, e }, /*private key*/{ n, d } };
		string text = readFile(argv[2]);

		string encryptedText = encryptText(text, rsaKeys.publicKey);
		string decryptedText = decryptText(encryptedText, rsaKeys.privateKey);

		writeInFile(encryptedText, argv[3]);
		writeInFile(decryptedText, argv[4]);
	}
	catch (string err)
	{
		cerr << err << endl;
		return 1;
	}
	return 0;
}