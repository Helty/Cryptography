#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <windows.h>

namespace
{
	struct PrimalPair
	{
		uint64_t p;
		uint64_t q;
	};
	struct PublicKey
	{
		uint64_t n;
		uint64_t e;
	};
	struct PrivateKey
	{
		uint64_t n;
		uint64_t d;
	}; 
		
	struct RSAKeys
	{
		PublicKey publicKey;
		PrivateKey privateKey;
	};

	using namespace std;
	using BigArray = vector<uint64_t>;
	using ArrayByte = vector<byte>;

	const int SIZE_TWO_BYTE = 65535;
}

string readFile(const char* filename);
void writeInFile(BigArray const& text, const char* filename);
BigArray getPrimalsNumberFromFile(const char* primalsFile);
PrimalPair getPairPrimalNumber(BigArray& primals);
uint64_t calcModule(PrimalPair const& primalPair);
uint64_t calcEuler(PrimalPair const& primalPair);
uint64_t calcSecretExponent(uint64_t e, uint64_t euler);
uint64_t calcPublicExponent(uint64_t euler);
uint64_t greatestCommonDivisor(uint64_t e, uint64_t euler);
uint64_t modPow(uint64_t base, uint64_t pov, uint64_t modul);
uint16_t concatByte(byte firstByte, byte secondByte);
BigArray encryptText(string const& text, PublicKey key);
BigArray encryptedTextByTwoByte(string const& text, PublicKey key);
BigArray encryptedTextByOneByte(string const& text, PublicKey key);
uint64_t encrypt(uint64_t code, PublicKey key);
BigArray decryptText(BigArray const& text, PrivateKey key);
BigArray decryptedTextByTwoByte(BigArray const& text, PrivateKey key);
BigArray decryptedTextByOneByte(BigArray const& text, PrivateKey key);
uint64_t decrypt(uint64_t code, PrivateKey key);
void ArgChecking(int argc);
void StartRSA(char* argv[]);
ArrayByte getTextToEncrypt(const char* textFile);
RSAKeys getPublicAndPrivateKeys(const char* primalFile);
PrimalPair GetPrimalPair(const char* primalFile);

//работа с файлами
string readFile(const char* filename)
{
	ifstream fileIn(filename, std::ios::binary);
	string str((std::istreambuf_iterator<char>(fileIn)),
		std::istreambuf_iterator<char>());
	fileIn.close();
	return str;
}
void writeInFile(BigArray const& text, const char* filename)
{
	std::ofstream file(filename);
	for (uint64_t elem : text)
	{
		file << (unsigned char)elem;
	}
	file.close();
}

//подсёт p q n Ф(n) e d
BigArray getPrimalsNumberFromFile(const char* primalsFile)
{
	BigArray result;
	string primals = readFile(primalsFile);
	istringstream iss(primals);
	string item;
	while (getline(iss, item, ' '))
	{
		result.push_back(stoi(item));
	}
	return result;
}
PrimalPair getPairPrimalNumber(BigArray& primals)
{
	if (primals.empty())
	{
		throw invalid_argument("Primals numbers is out.");
	}

	PrimalPair result{};

	result.p = primals.back();
	primals.pop_back();
	result.q = primals.back();
	primals.pop_back();

	return result;
}
uint64_t calcModule(PrimalPair const& primalPair)
{
	uint64_t n = (primalPair.p * primalPair.q);
	if (n < 255)
	{
		throw logic_error("Pick up another set (p, q >= 255), for example 17 and 19. To correctly encrypt two characters, you need (p*q >= 65535).");
	}
	return n;
}
uint64_t calcEuler(PrimalPair const& primalPair)
{
	return ((primalPair.p - 1) * (primalPair.q - 1));
}
uint64_t calcSecretExponent(uint64_t e, uint64_t euler)
{
	uint64_t d;
	uint64_t k = 1;

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
uint64_t calcPublicExponent(uint64_t euler)
{
	uint64_t e;

	for (e = 2; e < euler; e++)
	{
		if (greatestCommonDivisor(e, euler) == 1) return e;
	}

	throw logic_error("Non public exponent");
}

//вспомогательные функции
uint64_t greatestCommonDivisor(uint64_t e, uint64_t euler)
{
	while (e > 0)
	{
		uint64_t myTemp;

		myTemp = e;
		e = euler % e;
		euler = myTemp;
	}

	return euler;
}
uint64_t modPow(uint64_t base, uint64_t pov, uint64_t modul)
{
	uint64_t res = 1;
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
uint16_t concatByte(byte firstByte, byte secondByte)
{
	return firstByte << 8 | secondByte;
}

//шифрование
BigArray encryptText(string const& text, PublicKey key)
{
	return (key.n >= SIZE_TWO_BYTE) ? encryptedTextByTwoByte(text, key)
		: encryptedTextByOneByte(text, key);
}
BigArray encryptedTextByTwoByte(string const& text, PublicKey key)
{
	BigArray result;
	for (size_t i = 0; i < text.size(); i += 2)
	{
		byte next = ((text.size() % 2 != 0) and (text[i] == text.back())) ? 0 : text[i + 1];
		result.push_back(encrypt(concatByte(text[i], next), key));
	}
	return result;
}
BigArray encryptedTextByOneByte(string const& text, PublicKey key)
{
	BigArray result;
	for (size_t i = 0; i < text.size(); i++) result.push_back(encrypt((byte)text[i], key));
	return result;
}
uint64_t encrypt(uint64_t code, PublicKey key)
{
	return modPow(code, key.e, key.n);
}

//дешифрование
BigArray decryptText(BigArray const& text, PrivateKey key)
{
	return (key.n >= SIZE_TWO_BYTE) ? decryptedTextByTwoByte(text, key)
		: decryptedTextByOneByte(text, key);
}
BigArray decryptedTextByTwoByte(BigArray const& text, PrivateKey key)
{
	BigArray result;

	for (size_t i = 0; i < text.size(); i ++)
	{
		uint64_t decryptBytes = decrypt(text[i], key);
		result.push_back(decryptBytes >> 8);
		result.push_back((byte)decryptBytes);
	}

	return result;
}
BigArray decryptedTextByOneByte(BigArray const& text, PrivateKey key)
{
	BigArray result;
	for (size_t i = 0; i < text.size(); i++) result.push_back(decrypt(text[i], key));
	return result;
}
uint64_t decrypt(uint64_t code, PrivateKey key)
{
	return modPow(code, key.d, key.n);
}

void ArgChecking(int argc)
{
	if (argc != 5)
	{
		cout << "Usage: " << endl
			<< "\tcrypt.exe <file primal> <file text> <file encryptText> <file decryptText>" << endl
			<< "\t\t<file primal> - path to file, where stored primal numbers." << endl
			<< "\t\t<file text> - path to file, where stored text to encrypt." << endl
			<< "\t\t<file encryptText> - path to file, where will be encryption text." << endl
			<< "\t\t<file decryptText> - path to file, where will be decryption text." << endl;

		throw invalid_argument("Invalid arguments count.");
	}
}
void StartRSA(char* argv[])
{
	RSAKeys rsaKeys = getPublicAndPrivateKeys(argv[1]);
	string text = readFile(argv[2]);

	BigArray encryptedText = encryptText(text, rsaKeys.publicKey);
	BigArray decryptedText = decryptText(encryptedText, rsaKeys.privateKey);
	
	writeInFile(encryptedText, argv[3]);
	writeInFile(decryptedText, argv[4]);
}
ArrayByte getTextToEncrypt(const char* textFile)
{
	ArrayByte result;
	string textFromFile = readFile(textFile);

	for (byte letter : textFromFile)
	{
		result.push_back(letter);
	}

	return result;
}
RSAKeys getPublicAndPrivateKeys(const char* primalFile)
{
	PrimalPair primalPair = GetPrimalPair(primalFile);

	uint64_t n = calcModule(primalPair);
	uint64_t Euler = calcEuler(primalPair);
	uint64_t e = calcPublicExponent(Euler);
	uint64_t d = calcSecretExponent(e, Euler);

	return { /*public key*/{ n, e }, /*private key*/{ n, d } };
}
PrimalPair GetPrimalPair(const char* primalFile)
{
	BigArray primals = getPrimalsNumberFromFile(primalFile);
	return getPairPrimalNumber(primals);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		ArgChecking(argc);
		StartRSA(argv);
	}
	catch (const exception err)
	{
		cerr << err.what() << endl;
		return 1;
	}
	return 0;
}