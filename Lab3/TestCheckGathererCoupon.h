#pragma once
#include <vector>
#include <iostream>
#include <map>

std::vector<std::string> GetSubSequencesGathererCoupon(std::string const& bitSequence);

std::map<uint16_t, uint16_t> GetLengthOfSubSequenceGathererCoupon(std::vector<std::string> const& subSequence, uint16_t t);

double ChiSquareGathererCoupon(uint16_t t, std::map<uint16_t, uint16_t>& lengthOfSubSequence, double vSum, std::vector <double> const& pVector);

double GetVSum(std::map<uint16_t, uint16_t>& vMap);

std::vector<double> GetPVector(uint16_t t);

void StartCheckTestGathererCoupon(std::string const& bitSequence);
