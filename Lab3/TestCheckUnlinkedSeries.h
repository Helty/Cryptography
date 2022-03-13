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
#include <boost/math/distributions/inverse_chi_squared.hpp>

const size_t BIT_COUNT_FOR_NUMBER = 4;

double ChiSquareUnlinkedSeries(std::map<std::string, uint16_t> seriesCounter, std::string bitSequence, uint16_t m);
std::map<std::string, uint16_t> SetAll—ombinationsOfSerial(uint16_t m);
std::map<std::string, uint16_t> GetSeriesCounter(std::string bitSequence, uint16_t m);
void StartCheckUnlinkedSeries(std::string bitSequence);
