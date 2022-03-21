#include "TestCheckIntervals.h"

double ChiSquareIntervals(std::map<uint16_t, int16_t> const& counterIntervals, Interval const& interval, uint16_t freedomDegree)
{
	double result = 0;
	uint64_t totalNumberIntervals = 0;

	for (auto const& intervalLength : counterIntervals) totalNumberIntervals += intervalLength.second;

	for (auto const& intervalLength : counterIntervals)
	{
		double first = 1.5;
		double second = totalNumberIntervals * first;
		double third = pow(1 - first, intervalLength.first);
		double four = second * third;
		result += (pow((intervalLength.second - four), 2)) / four;
	}
	return abs(result);
}

std::map<uint16_t, int16_t> GetCounterIntervals(std::vector<int16_t> const& sequenceIntervalLengths, uint16_t freedomDegree)
{
	std::map<uint16_t, int16_t> result;
	for (uint16_t i = 0; i != freedomDegree; i++) result[i] = 0;
	for (int16_t num : sequenceIntervalLengths)
	{
		if (num <= 0)
		{
			result[0]++;
			continue;
		}
		result[num]++;
	}
	return result;
}

std::vector<int16_t> GetSequenceIntervalLengths(std::string const& bitSequence)
{
	std::vector<int16_t> result;
	int16_t counter = 1;
	for (size_t i = 0; i != bitSequence.size(); i++)
	{
		if (i == 0) continue;
		if (bitSequence[i - 1] != bitSequence[i])
		{
			result.push_back(counter - 2);
			counter = 1;
			continue;
		}
		counter++;
		if ((i + 1) == bitSequence.size()) result.push_back(counter - 2);
	}
	return result;
}

void StartCheckIntervals(std::string const& bitSequence)
{
	Interval interval = { 0, 1 };
	uint16_t freedomDegree = 10;
	std::vector<int16_t> sequenceIntervalLengths = GetSequenceIntervalLengths(bitSequence);
	std::map<uint16_t, int16_t> counterIntervals = GetCounterIntervals(sequenceIntervalLengths, freedomDegree);
	std::cout << "CheckIntervals(степени свободы: " << freedomDegree << "): " << ChiSquareIntervals(counterIntervals, interval, freedomDegree) << std::endl;
}