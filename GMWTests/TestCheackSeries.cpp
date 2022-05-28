#include "TestCheackSeries.h"

void CountingOnesZeros(std::string const& bitSequence)
{
    std::cout << "\tseriesByOne: " << std::endl;
    std::cout << "\t0: " << count(bitSequence.begin(), bitSequence.end(), '0') << std::endl
        << "\t1: " << count(bitSequence.begin(), bitSequence.end(), '1') << std::endl;
}

void SeriesByTwo(std::string const& bitSequence)
{
    std::vector<std::string> pair;
    std::map<std::string, int> result;

    std::cout << std::endl << "\tseriesByTwo: ";
    for (int i = 0; i != bitSequence.size(); i += 2)
    {
        std::string s1 = bitSequence.substr(i, 2);
        if (s1.size() != 2) break;
        pair.push_back(s1);
    }
    std::cout << std::endl;
    for (auto const& elem: pair)
    {
        result[elem]++;
    }
    for (auto const& item : result)
    {
        std::cout << "\t" << item.first << ": " << item.second << std::endl;
    }
}

void SeriesByThree(std::string const& bitSequence)
{
    std::map<std::string, int> counter3;
    std::cout << std::endl << "\tseriesByThree: ";
    for (int i = 0; i != bitSequence.size(); i += 3)
    {
        std::string s1 = bitSequence.substr(i, 3);
        if (s1.length() == 3)  continue;
        break;

    };

    std::cout << std::endl;
    for (int i = 0; i != bitSequence.size(); i += 3)
    {
        std::string s1 = bitSequence.substr(i, 3);
        if (s1.length() == 3) 
        {
            counter3[s1] += 1;
            continue;
        }
        break;

    }
    for (auto& item : counter3)
    {
        std::cout << "\t" << item.first << ": " << item.second << std::endl;
    }
}

void SeriesByFour(std::string const& bitSequence)
{
    std::cout << std::endl << "\tseriesByFour: ";
    std::vector<std::string> pair;
    std::map<std::string, int> counter4;

    for (int i = 0; i != bitSequence.size(); i += 4)
    {
        std::string s1 = bitSequence.substr(i, 4);
        if (s1.size() != 4) break;
        pair.push_back(s1);
    }

    std::cout << std::endl;

    for (auto const& elem : pair)
    {
        counter4[elem]++;
    }

    for (auto& item : counter4)
    {
        std::cout << "\t" << item.first << ": " << item.second << std::endl;
    }
}

void StartCheackSeries(std::string const& bitSequence)
{
    std::cout << "CheackSeries: " << std::endl;
    CountingOnesZeros(bitSequence);
    SeriesByTwo(bitSequence);
    SeriesByThree(bitSequence);
    SeriesByFour(bitSequence);
}