#pragma once
#include <vector>
#include <iostream>
#include <string>

struct Point
{
	uint16_t x;
	uint16_t y;
};

std::vector<Point> getPoints(std::string NumSequence);
void StartCheckFlatDistribution(std::string const& NumSequence);