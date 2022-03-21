#include "TestCheckSpectralAndExpCriterial.h"

void PrintCheckExpCriterial(int64_t N, std::vector<std::complex<double>> const& p)
{
	std::cout << "CheckExpCriterial: " << std::endl;
	for (int64_t i = 0; i < N; i++)
	{
		std::cout << "\t" << std::fixed << std::setprecision(3) << p[i];
		if (i != N - 1) std::cout << std::endl;
	}
	std::cout << std::endl;
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