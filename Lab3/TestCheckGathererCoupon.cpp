#include "TestCheckGathererCoupon.h"
#include "alglib-cpp/specialfunctions.h"

std::vector<std::string> GetSubSequencesGathererCoupon(std::string const& bitSequence)
{
	std::vector<std::string> result;
	std::string sub = "";

	bool flagZero = false;
	bool flagOne = false;

	for (size_t i = 0; i != bitSequence.size(); i += 1)
	{
		if ((bitSequence[i] - '0') == 1)
		{
			sub.push_back(bitSequence[i]);
			flagOne = true;
		}
		else
		{
			sub.push_back(bitSequence[i]);
			flagZero = true;
		}

		if ((flagOne == true) and (flagZero == true))
		{
			result.push_back(sub);
			sub = "";
			flagZero = false;
			flagOne = false;
		}
	}
	return result;
}

std::map<uint16_t, uint16_t> GetLengthOfSubSequenceGathererCoupon(std::vector<std::string> const& subSequence, uint16_t t)
{
	std::map<uint16_t, uint16_t> result;
	for (uint16_t i = 2; i <= t; i++) result[i] = 0;
	for (const auto& sub : subSequence)
	{
		if (sub.size() > t) continue;
		result[(uint16_t)sub.size()]++;
	}
	return result;
}

double ChiSquareGathererCoupon(uint16_t t, std::map<uint16_t, uint16_t>& lengthOfSubSequence, double vSum, std::vector <double> const& pVector)
{
	double result = 0;
	for (uint16_t i = 1; i <= t; i++)
	{
		double first = vSum * pVector[i - 1];
		if (first != 0) result += pow(lengthOfSubSequence[i] - first, 2) / first;
	}
	return result;
}

double GetVSum(std::map<uint16_t, uint16_t> & vMap)
{
	double vSum = 0;
	for (auto const& pair : vMap) vSum += pair.second;
	vMap[1] = 1;
	return vSum;
}

std::vector<double> GetPVector(uint16_t t)
{
	std::vector<double> pVector;
	for (size_t i = 1; i < t; i++) pVector.push_back(1);
	pVector.push_back(0);
	return pVector;
}

void StartCheckTestGathererCoupon(std::string const& bitSequence)
{
	uint16_t t = 5;
	std::vector<std::string> subSequence = GetSubSequencesGathererCoupon(bitSequence);
	std::map<uint16_t, uint16_t> vMap = GetLengthOfSubSequenceGathererCoupon(subSequence, t);
	double vSum = GetVSum(vMap);
	std::vector <double> pVector = GetPVector(t);
	std::cout << "CheckGathererCoupon: " << alglib::chisquarecdistribution(t - 2 + 1, ChiSquareGathererCoupon(t, vMap, vSum, pVector)) << std::endl;
}