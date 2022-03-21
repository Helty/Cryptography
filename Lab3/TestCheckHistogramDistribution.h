#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

struct SequenceHistogram
{
    std::string sequence_string;
    std::vector<int> sequence_full;
    std::vector<char> sequence_full_char;
    std::vector<int> sequence_unique;
    std::vector<char> sequence_unique_char;
    std::vector<int> sequence_ascii;
};

void PrintResultHistogramDistribution(std::vector<int> const& ASCIIBitVector);
void PrintResultHistogramDistribution(SequenceHistogram const& seq);

bool IsBitSequenceHistogramDistribution(std::string const& sequence);

std::vector<int> GetSequenceFromFileHistogramDistribution(std::string const& seqStr);
std::vector<char> GetSequenceFromFileCharHistogramDistribution(std::string seqStr);

std::vector<int> MakeSeqWithUniqueElemsHistogramDistribution(std::vector<int> const& vect0);
std::vector<char> MakeSeqWithUniqueElemsCharHistogramDistribution(std::vector<char> const& vect0);

std::vector<int> MakeUniqueASCIISeqHistogramDistribution(std::string const& vect0);
std::vector<int> MakeUniqueASCIISeqHistogramDistribution(std::vector<char> const& vect0);

void StartCheckHistogramDistribution(std::string const& bitSequence);