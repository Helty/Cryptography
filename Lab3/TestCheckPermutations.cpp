#include "TestCheckPermutations.h"

std::vector<std::string> getSubsequencesPermutations(std::string bitSequence, uint16_t t)
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
std::map<std::string, uint16_t> getVMapPermutations(std::vector<std::string> subSequences)
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
double ChiSquarePermutations(std::map<std::string, uint16_t> vMap, std::vector<std::string> subSequences, uint16_t t)
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
	std::vector<std::string> subSequences = getSubsequencesPermutations(bitSequence, t);
	std::map<std::string, uint16_t> vMap = getVMapPermutations(subSequences);
	std::cout << ChiSquarePermutations(vMap, subSequences, t) << std::endl;
}