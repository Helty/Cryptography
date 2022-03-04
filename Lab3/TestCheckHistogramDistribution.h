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

void PrintResult(std::vector<int> ASCIIBitVector);
void PrintResult(SequenceHistogram const& seq);

bool isBitSequence(std::string const& sequence);

std::vector<int> getSequenceFromFile(std::string seqStr);
std::vector<char> getSequenceFromFile_char(std::string seqStr);

std::vector<int> makeSeqWithUniqueElems(std::vector<int> vect0);
std::vector<char> makeSeqWithUniqueElems_char(std::vector<char> vect0);

std::vector<int> makeUniqueASCIISeq(std::string vect0);
std::vector<int> makeUniqueASCIISeq(std::vector<char> vect0);

void StartCheckHistogramDistribution(std::string const& bitSequence);