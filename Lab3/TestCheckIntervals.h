#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>

struct Interval
{
	uint16_t alpha;
	uint16_t betta;
};

double ChiSquare(std::map<uint16_t, int16_t> counterIntervals, Interval interval, uint16_t freedomDegree);
std::map<uint16_t, int16_t> getCounterIntervals(std::vector<int16_t> const& sequenceIntervalLengths, uint16_t freedomDegree);
std::vector<int16_t> getSequenceIntervalLengths(std::string const& bitSequence);
void StartCheckIntervals(std::string bitSequence);