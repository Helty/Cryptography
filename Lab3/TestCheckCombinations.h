#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

std::vector<std::vector<uint16_t>> getSubsequences(std::string bitSequence, uint16_t t);
std::map<uint16_t, uint16_t> getLengOfSubsequences(std::string bitSequence, uint16_t t);
void StartCheckCombinations(std::string bitSequence);