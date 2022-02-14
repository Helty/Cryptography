#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

string bitXor(string const& str1, string const& str2)
{
	string result;

	for (int i = 0; i < str1.length(); i++)
	{
		result += (str1.at(i) ^ str2.at(i));
	}
	return result;
}

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

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	if (argc != 4)
	{
		cout << "Invalid arguments count" << endl
			<< "Usage: " << endl 
			<< "\tcrypt.exe <file to crypt> <key file> <encrypt file>" << endl
			<< "\t\t<file to crypt> - path to file, where stored data for encryption." << endl
			<< "\t\t<key file> - path to file, where stored key." << endl
			<< "\t\t<encrypt file> - path to file, where will be encryption text." << endl;
		return 1;
	}
	
	try
	{
		string code = readFile(argv[1]);
		string key = readFile(argv[2]);

		if (key.size() != code.size())
		{
			cerr << "Key size: " << key.size() << endl;
			cerr << "TextToCrypt size: " << code.size() << endl << endl;
			throw std::logic_error("Failed key size");
		}

		writeInFile(bitXor(code, key), argv[3]);
	}
	catch (string err)
	{
		cerr << err << endl;
		return 1;
	}

	return 0;
}