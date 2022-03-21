#include "TestCheckCombinations.h"

std::vector<std::string> GetSubsequencesCombinations(std::string const& bitSequence, uint16_t t)
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

std::map<uint16_t, uint16_t> GetLengOfSubsequencesCombinations(std::vector<std::string> const& subSequences)
{
	std::map<uint16_t, uint16_t> result;

	for (uint16_t i = 1; i <= 2; i++) result[i] = 0;

	uint16_t counterOne = 0;
	uint16_t counterZero = 0;

	for (const auto& v : subSequences)
	{
		for (const auto& num : v)
		{
			if ((num - '0') == 1) counterOne++;
			if ((num - '0') == 0) counterZero++;
		}
		if ((counterOne != 0) and (counterZero != 0)) result[2]++;
		if ((counterOne == 0) or (counterZero == 0)) result[1]++;
		counterOne, counterZero = 0;
	}
	return result;
}

void StartCheckCombinations(std::string const& bitSequence)
{
	double n = static_cast<double>(bitSequence.size());
	uint16_t t = 4;
	std::vector<std::string> subSequences = GetSubsequencesCombinations(bitSequence, t);
	std::map<uint16_t, uint16_t> mapLengOfSubsequences = GetLengOfSubsequencesCombinations(subSequences);
	std::cout << "CheckCombinations(степени свободы: " << subSequences.size()-1 << "): " << pow((mapLengOfSubsequences[1] - n / t), 2) / (n / t) << std::endl;
}