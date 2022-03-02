#include <iostream>
#include <Windows.h>

#include "TestUnlinkedSeries.h"
#include "TestCheckIntervals.h"
#include "TestCheckCombinations.h"
#include "TestGathererCoupon.h"
#include "TestCheckPermutations.h"

std::string getSequenceFromFile(const char* file)
{
	std::ifstream fileIn(file);
	std::string str((std::istreambuf_iterator<char>(fileIn)),
		std::istreambuf_iterator<char>());
	fileIn.close();
	return str;
}
std::string getBinStringOfNumber(uint64_t number)
{
	return std::bitset<BIT_COUNT_FOR_NUMBER>(number).to_string();
}
std::string getBitSequenceFromString(std::string const& sequence)
{
	std::string result;
	std::vector<std::string> numbersOfSequence;
	boost::split(numbersOfSequence, sequence, boost::is_any_of("\t ,-"));
	for (std::string number : numbersOfSequence)
	{
		result += getBinStringOfNumber(stoi(number));
	}
	return result;
}
std::string getBitSequenceFromFile(const char* file)
{
	std::string sequence = getSequenceFromFile(file);
	return getBitSequenceFromString(sequence);
}

void StartTests(std::string const& bitSequence)
{
	StartCheckUnlinkedSeries(bitSequence);
	StartCheckIntervals(bitSequence);
	StartCheckCombinations(bitSequence);
	StartCheckTestGathererCoupon(bitSequence);
	StartCheckPermutations(bitSequence);
}
void ArgChecking(int argc)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << std::endl
			<< "\tLab3.exe <bit sequence>" << std::endl
			<< "\t\t<bit sequence> - continuous sequence of 0s and 1s." << std::endl;
		throw std::invalid_argument("Invalid arguments count.");
	}
}

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		ArgChecking(argc);
		//std::string bitSequence = getBitSequenceFromFile(argv[1]);
		StartTests(argv[1]);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}
