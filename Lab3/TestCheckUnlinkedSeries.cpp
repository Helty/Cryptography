#include "TestCheckUnlinkedSeries.h"

void StartCheckUnlinkedSeries(std::string bitSequence)
{
	uint16_t m = 3;
	std::map<std::string, uint16_t> seriesCounter = GetSeriesCounter(bitSequence, m);
	boost::math::inverse_chi_squared(0.9, 50);
	std::cout << "CheckUnlinkedSeries(ÒÚÂÔÂÌ¸ Ò‚Ó·Ó‰˚ " << pow(2,m) - 1 << "): " << ChiSquareUnlinkedSeries(seriesCounter, bitSequence, m) << std::endl;
}

std::map<std::string, uint16_t> GetSeriesCounter(std::string bitSequence, uint16_t m)
{
	std::map<std::string, uint16_t> result = SetAll—ombinationsOfSerial(m);
	for (size_t i = 0; i != bitSequence.size(); i += m)
	{
		std::string sub = bitSequence.substr(i, m);
		if (sub.size() != m) break;
		result[sub] += 1;
	}
	return result;
}
std::map<std::string, uint16_t> SetAll—ombinationsOfSerial(uint16_t m)
{
	std::map<std::string, uint16_t> result;

	uint64_t number = (uint64_t)pow(m, 2);
	std::string strbits;
	for (uint32_t i = 0; i <= number; i++)
	{
		boost::dynamic_bitset<> bits(m, i);
		boost::to_string(bits, strbits);
		result[strbits] = 0;
	}
	return result;
}
double ChiSquareUnlinkedSeries(std::map<std::string, uint16_t> seriesCounter, std::string bitSequence, uint16_t m)
{
	std::vector<uint16_t> valueFromMap;
	for (std::map<std::string, uint16_t>::iterator it = seriesCounter.begin(); it != seriesCounter.end(); ++it)
	{
		valueFromMap.push_back(it->second);
	}
	double divider = (bitSequence.size() / m) * (1 / pow(2, m));
	double b = 1 / divider;
	double sum = 0;
	for (size_t i = 0; i != valueFromMap.size(); i++)
	{
		sum += pow(valueFromMap[i] - divider, 2);
	}
	return  b * sum;
}

