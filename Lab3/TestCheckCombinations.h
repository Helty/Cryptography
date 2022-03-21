#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

std::vector<std::string> GetSubsequencesCombinations(std::string const& bitSequence, uint16_t t);

std::map<uint16_t, uint16_t> GetLengOfSubsequencesCombinations(std::vector<std::string> const& subSequences);

void StartCheckCombinations(std::string const& bitSequence);