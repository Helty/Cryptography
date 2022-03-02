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

std::vector<std::string> getSubsequencesPerm(std::string bitSequence, uint16_t t)
{
	std::vector<std::string> result;
	for (size_t i = 0; i != bitSequence.size(); i += t)
	{
		std::string sub = bitSequence.substr(i, t);
		if (sub.size() != t) break;
		result.push_back(sub);
	}
	return result;
}
std::map<std::string, uint16_t> getVMap(std::vector<std::string> subSequences)
{
	std::map<std::string, uint16_t> result;
	if (subSequences.size() < 2)
	{
		throw std::length_error("Sequences is so little.");
	}
	for (size_t i = 0; i < 3; i++)
	{
		result[subSequences[i]] = 0;
		for (size_t j = 0; j != subSequences.size(); j++)
		{
			if (subSequences[i] == subSequences[j]) result[subSequences[i]]++;
		}
	}
	return result;
}
double Fact(double n)
{
	return (n > 1) ? n * Fact(n - 1) : n;
}
double ChiSquare(std::map<std::string, uint16_t> vMap, std::vector<std::string> subSequences, uint16_t t)
{
	double result = 0;
	double length = subSequences.size();
	double first = ((double)(length / t) * (1.0 / Fact(t)));
	for (size_t i = 0; i < t; i++)
	{
		if (first != 0)
		{
			result += pow(vMap[subSequences[i]] - first, 2);
		}
	}
	return result / first;
}
void StartCheckPermutations(std::string bitSequence)
{
	uint16_t t = 2;
	std::vector<std::string> subSequences = getSubsequencesPerm(bitSequence, t);
	std::map<std::string, uint16_t> vMap = getVMap(subSequences);
	std::cout << ChiSquare(vMap, subSequences, t) << std::endl;
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
