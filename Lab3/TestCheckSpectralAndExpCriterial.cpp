#include "TestCheckSpectralAndExpCriterial.h"

void PrintCheckExpCriterial(int64_t N, std::vector<std::complex<double>> const& p)
{
	std::cout << "CheckExpCriterial: count of modulus complex number: " << p.size() << std::endl;
	std::vector<double> modulusOfComlex;
	for (int64_t i = 0; i < N; i++)
	{
		modulusOfComlex.push_back(sqrt(pow(p[i].real(), 2) + pow(p[i].imag(), 2)));
	}
	std::map<double, uint64_t> result;

	for (auto const& item : modulusOfComlex)
	{
		result[item]++;
	}
	
	for (auto const& item : result)
	{
		std::cout << "\t" << item.first << ": " << item.second << std::endl;
	}
}

void PrintCheckSpectralCriterial(int64_t N, std::vector<std::complex<double>> const& p)
{
	std::complex<double> e, buf = 0;
	for (int64_t m = 0; m < N; m++) buf += pow(abs(p[m]) - sqrt(N), 2);
	e = sqrt(buf);
	std::cout << "CheckSpectralCriterial: " << std::endl;
	std::cout << "\te = " << e.real() << std::endl;
}

void StartCheckSpectralAndExpCriterial(std::string const& bitSequence)
{
	std::vector<std::complex<double>> u;
	int64_t N = bitSequence.size();
	std::vector<std::complex<double>> p;

	//u
	for (int64_t i = 0; i < N; i++) u.push_back((bitSequence[i] == '1') ? -1 : 1);

	//p
	for (int64_t m = 0; m < N; m++)
	{
		p.push_back(0);
		for (int64_t n = 0; n < N; n++)
		{
			p[m] += std::complex<double>(u[n]) * std::complex<double>(cos(-2 * M_PI * m * n / N), sin(-2 * M_PI * m * n / N));
		}
	}
	PrintCheckExpCriterial(N, p);

	//e
	PrintCheckSpectralCriterial(N, p);
}