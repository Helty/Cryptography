#include "TestCheackMonotone.h"

std::vector<Area> MonotonicityCheck(std::string const& bitSequense)
{
    std::vector<Area> res;
    Area monoArea={};
    monoArea.areaLen = 1;
    bool lastUp = false;
    for (size_t i = 0; i != bitSequense.size();)
    {
        if (bitSequense[i] == bitSequense[i + 1])
        {
            monoArea.subSequense.push_back(bitSequense[i]);
            monoArea.areaLen++;
            i++;
            continue;
        }
        monoArea.subSequense.push_back(bitSequense[i]);
        monoArea.areaLen++;
        i++;

        while ((bitSequense[i+1] != std::string::npos) and (bitSequense[i] == bitSequense[i + 1]))
        {
            monoArea.subSequense.push_back(bitSequense[i]);
            monoArea.areaLen++;
            i++;
        }
        monoArea.subSequense.push_back(bitSequense[i]);
        monoArea.areaLen--;
        monoArea.upOrDown = (monoArea.subSequense[0] > monoArea.subSequense[monoArea.subSequense.size() - 1]) ? "Убывает" : "Возрастает";
        monoArea.subSequense.erase(monoArea.subSequense.begin() + monoArea.areaLen, monoArea.subSequense.end());
        res.push_back(monoArea);
        if (monoArea.upOrDown == "Возрастает") lastUp = true;
        monoArea = {};
        monoArea.areaLen = 1;
        if ((i + 1) == bitSequense.size())
        {
            break;
        }
    }
    return res;
}

void PrintResultMonotone(std::vector<Area> const& res)
{
    std::cout << "CheackMonotone: " << std::endl;
    for (auto item : res)
    {
        std::cout << "\t" << item.upOrDown << ":" << item.areaLen << "; " << std::endl;
    }
}

void StartCheackMonotone(std::string const& bitSequence)
{
    std::vector<Area> result = MonotonicityCheck(bitSequence);
    PrintResultMonotone(result);
}