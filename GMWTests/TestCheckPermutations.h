#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

std::vector<std::string> GetSubsequencesPermutations(std::string const& bitSequence, uint16_t t);

std::map<std::string, uint16_t> GetVMapPermutations(std::vector<std::string> const& subSequences);

double Fact(double n);

double ChiSquarePermutations(std::map<std::string, uint16_t>& vMap, std::vector<std::string> const& subSequences, uint16_t t);

void StartCheckPermutations(std::string const& bitSequence);
