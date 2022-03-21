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

double ChiSquareIntervals(std::map<uint16_t, int16_t> const& counterIntervals, Interval const& interval, uint16_t freedomDegree);

std::map<uint16_t, int16_t> GetCounterIntervals(std::vector<int16_t> const& sequenceIntervalLengths, uint16_t freedomDegree);

std::vector<int16_t> GetSequenceIntervalLengths(std::string const& bitSequence);

void StartCheckIntervals(std::string const& bitSequence);