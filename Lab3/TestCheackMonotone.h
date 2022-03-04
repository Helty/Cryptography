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

std::vector<Area> monotonicityCheck(std::string);
void printoutResult(std::vector<Area> res);
void StartCheackMonotone(std::string bitSequence);