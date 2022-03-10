#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

struct DegreeOfPolynome
{
	std::vector<uint32_t> 
};

struct GMWInfo
{
	uint16_t k;
	uint16_t s;
	uint32_t m;
	uint64_t N;
};

void ValidateArguments(std::vector<std::string> argv)
{
	if (stoi(argv[1]) <= 2 or stoi(argv[2]) <= 3)
	{
		throw std::invalid_argument("error k or s.");
	}
}
void CheckAgruments(int argc)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << std::endl 
			<< "\tLab3.2.exe <k> <s>" << std::endl
			<< "\t\t<k> - k should be >= 2." << std::endl
			<< "\t\t<s> - s should be >= 3." << std::endl;

		throw std::invalid_argument("Invalid arguments count.");
	}
}

void StartGMW(std::vector<std::string> argv)
{
	GMWInfo startGMWInfo{};
	startGMWInfo.k = stoi(argv[1]);
	startGMWInfo.s = stoi(argv[2]);
	startGMWInfo.m = startGMWInfo.k * startGMWInfo.s;
	startGMWInfo.N = pow(2, startGMWInfo.m) - 1;

	std::string polinome = StartProcess(Polinome.exe, "- 2 - 4");



}

int main(int argc, std::vector<std::string> argv)
{
	std::cout << "GMW-последовательность" << std::endl << std::endl;

	try
	{
		CheckAgruments(argc);
		ValidateArguments(argv);
		StartGMW(argv);
	}
	catch (std::exception & error)
	{
		std::cout << error.what() << std::endl;
		return 1;
	}
}
