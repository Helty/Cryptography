#pragma once
#include <iostream>
#include <string>
#include <vector>

struct Area
{
    std::string upOrDown;
    int areaLen;
    std::string subSequense;
};

std::vector<Area> MonotonicityCheck(std::string const& bitSequense);

void PrintResultMonotone(std::vector<Area> const& res);

void StartCheackMonotone(std::string const& bitSequence);