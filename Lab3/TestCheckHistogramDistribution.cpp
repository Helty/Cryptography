#include "TestCheckHistogramDistribution.h"

void PrintResult(SequenceHistogram const& seq) 
{
    std::cout << "" << std::endl;
    std::cout << std::endl << "" << seq.sequence_string << std::endl;

    for (int i = 0; i < seq.sequence_unique.size(); i++) 
    {
        std::cout << "" << seq.sequence_unique[i] << "" << seq.sequence_ascii[i] << ""
            << count(seq.sequence_full.begin(), seq.sequence_full.end(), seq.sequence_unique[i]) << std::endl;
    }
}
void PrintResult(std::vector<int> ASCIIBitVector)
{
    std::cout << "\t1 (49): " << count(ASCIIBitVector.begin(), ASCIIBitVector.end(), 49) << std::endl;
    std::cout << "\t0 (48): " << count(ASCIIBitVector.begin(), ASCIIBitVector.end(), 48) << std::endl;
}
bool isBitSequence(std::string const& sequence)
{
    return (std::all_of(sequence.begin(), sequence.end(),
        [](char i) {return ((i == '1') or (i == '0')); })) ? true : false;
}

std::vector<int> getSequenceFromFile(std::string seqStr) 
{
    std::vector<int> sequence;
    std::istringstream seqStrStream(seqStr);
    std::string elem;
    while (getline(seqStrStream, elem, ' ')) sequence.push_back(stoi(elem));
    return sequence;
}
std::vector<char> getSequenceFromFile_char(std::string seqStr) 
{
    seqStr.erase(remove(seqStr.begin(), seqStr.end(), ' '), seqStr.end());
    std::vector<char> sequence(seqStr.begin(), seqStr.end());
    return sequence;
}
std::vector<int> makeSeqWithUniqueElems(std::vector<int> vect0) 
{ 
    std::vector<int> vect;
    for (int item : vect0) 
    {
        if (vect.empty()) vect.push_back(item);
        else if (!(find(vect.begin(), vect.end(), item) != vect.end())) vect.push_back(item);
    }
    return vect;
}
std::vector<char> makeSeqWithUniqueElems_char(std::vector<char> vect0) 
{
    std::vector<char> vect;
    for (int item : vect0) 
    {
        if (vect.empty()) vect.push_back(item);
        else if (!(find(vect.begin(), vect.end(), item) != vect.end())) vect.push_back(item);
    }
    return vect;
}
std::vector<int> makeUniqueASCIISeq(std::string vect0) 
{
    std::vector<int> vect;
    for (char item : vect0) vect.push_back((int)item);
    return vect;
}
std::vector<int> makeUniqueASCIISeq(std::vector<char> vect0)
{
    std::vector<int> vect;
    for (char item : vect0) vect.push_back((int)item);
    return vect;
}

void StartCheckHistogramDistribution(std::string const& sequence)
{
    std::cout << "CheckHistogramDistribution: " << std::endl;
    if (isBitSequence(sequence))
    {
        std::vector<int> ASCIIBitVector = makeUniqueASCIISeq(sequence);
        PrintResult(ASCIIBitVector);
    }
    else
    {
        SequenceHistogram seq;
        seq.sequence_string = sequence;
        seq.sequence_full = getSequenceFromFile(seq.sequence_string);
        seq.sequence_full_char = getSequenceFromFile_char(seq.sequence_string);
        seq.sequence_unique = makeSeqWithUniqueElems(seq.sequence_full);
        seq.sequence_unique_char = makeSeqWithUniqueElems_char(seq.sequence_full_char);
        seq.sequence_ascii = makeUniqueASCIISeq(seq.sequence_unique_char);
        PrintResult(seq);
    }
}