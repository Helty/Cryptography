#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/exception/to_string.hpp>

enum class Serieslength
{
	TWO = 2,
	THREE = 3,
	FIVE = 5,
	SEVEN = 7
};
const size_t BIT_COUNT_FOR_NUMBER = 4;

double ChiSquare(std::map<std::string, uint16_t> seriesCounter, std::string bitSequence, uint16_t m);
std::map<std::string, uint16_t> SetAll—ombinationsOfSerial(uint16_t m);
std::map<std::string, uint16_t> GetSeriesCounter(std::string bitSequence, uint16_t m);
uint16_t ToInt(Serieslength m);
Serieslength GetSeriesLenght(size_t lengthBits);
void StartCheckUnlinkedSeries(std::string bitSequence);
