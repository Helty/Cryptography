#include "TestCheckPermutations.h"

std::vector<std::string> GetSubsequencesPermutations(std::string const& bitSequence, uint16_t t)
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

std::map<std::string, uint16_t> GetVMapPermutations(std::vector<std::string> const& subSequences)
{
	std::map<std::string, uint16_t> result;
	if (subSequences.size() < 2)
	{
		throw std::length_error("Sequences is so little.");
	}
	for (size_t i = 0; i < 3; i++)
	{
		if (i == subSequences.size()) break;
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

double ChiSquarePermutations(std::map<std::string, uint16_t>& vMap, std::vector<std::string> const& subSequences, uint16_t t)
{
	double result = 0;
	double length = (double)subSequences.size();
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

void StartCheckPermutations(std::string const& bitSequence)
{
	uint16_t t = 3;
	std::vector<std::string> subSequences = GetSubsequencesPermutations(bitSequence, t);
	std::map<std::string, uint16_t> vMap = GetVMapPermutations(subSequences);
	std::cout << "CheckPermutations(степени свободы: " << Fact(t) - 1 << "): " << ChiSquarePermutations(vMap, subSequences, t) << std::endl;
}