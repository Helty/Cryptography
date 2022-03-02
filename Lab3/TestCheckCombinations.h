#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

std::vector<std::string> getSubsequencesCombinations(std::string bitSequence, uint16_t t);
std::map<uint16_t, uint16_t> getLengOfSubsequencesCombinations(std::string bitSequence, uint16_t t);
void StartCheckCombinations(std::string bitSequence);