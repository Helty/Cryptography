#pragma once
#include <vector>
#include <iostream>
#include <map>

std::vector<std::string> getSubSequences(std::string bitSequence);
std::map<uint16_t, uint16_t> getLengthOfSubSequence(std::vector<std::string> subSequence, uint16_t t);
double ChiSquare(uint16_t t, std::map<uint16_t, uint16_t> lengthOfSubSequence, double vSum, std::vector <double> pVector);
double getVSum(std::map<uint16_t, uint16_t>& vMap);
std::vector<double> getPVector(uint16_t t);
void StartCheckTestGathererCoupon(std::string bitSequence);
