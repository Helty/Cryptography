#include <iostream>
#include <Windows.h>

#include "TestCheckUnlinkedSeries.h"
#include "TestCheckIntervals.h"
#include "TestCheckCombinations.h"
#include "TestCheckGathererCoupon.h"
#include "TestCheckPermutations.h"

#include "TestCheckFlatDistribution.h"
#include "TestCheackLinearComplexityProfile.h"
#include "TestCheackMonotone.h"
#include "TestCheackSeries.h"
#include "TestCheckHistogramDistribution.h"

#include "TestCheckSpectralAndExpCriterial.h"
#include "TestCheackCorrelation.h"

std::string getSequenceFromFile(const char* file)
{
	std::ifstream fileIn(file);
	std::string str((std::istreambuf_iterator<char>(fileIn)),
		std::istreambuf_iterator<char>());
	fileIn.close();
	return str;
}

void StartTests(std::string const& bitSequence)
{
	//оценочные тесты Хи-квадрат
	StartCheckUnlinkedSeries(bitSequence);
	StartCheckIntervals(bitSequence);
	StartCheckCombinations(bitSequence);
	StartCheckTestGathererCoupon(bitSequence);
	StartCheckPermutations(bitSequence);

	//графические тесты
	StartCheckHistogramDistribution(bitSequence); //разброс частот появления стремится к нулю.
	StartCheckFlatDistribution(bitSequence); //точки должны быть расположены без зависимостей, а хаотично
	StartCheackSeries(bitSequence); //разбросы между числом появлений нулей и единиц, между появлений серий пар каждого вида должны стремиться к нулю
	StartCheackMonotone(bitSequence); // чем больше длина, тем меньше вероятность, иначе последовательность не случайна.
	StartCheackLinearComplexityProfile(bitSequence); //должна стремиться к линии N/2, иначе не случайная

	//Разбирали на паре
	StartCheckSpectralAndExpCriterial(bitSequence); // должна быть минимальна, идеально 0.
	StartCheackCorrelation(bitSequence); //MF >= 6 - хорошо; MF <= 1 - плохо. R >= sqrt(N) - хорошо.

}
void ArgChecking(int argc)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << std::endl
			<< "\tLab3.exe <filepath>" << std::endl
			<< "\t\t<filepath> - continuous sequence of 0s and 1s." << std::endl;
		throw std::invalid_argument("Invalid arguments count.");
	}
}

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	try
	{
		ArgChecking(argc);
		std::string bitSequence = getSequenceFromFile(argv[1]);
		StartTests(bitSequence);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
}
