#include "TestCheckSpectralAndExpCriterial.h"

void PrintCheckExpCriterial(size_t N, std::complex<double>* p)
{
	std::cout << "CheckExpCriterial: {";
	for (int16_t i = 0; i < N; i++)
	{
		std::cout << std::fixed << std::setprecision(3) << p[i];
		if (i != N - 1) std::cout << ",";
	}
	std::cout << "}" << std::endl;
}
void PrintCheckSpectralCriterial(size_t N, std::complex<double>* p)
{
	std::complex<double> e, buf = 0;
	for (int16_t m = 0; m < N; m++) buf += pow(abs(p[m]) - sqrt(N), 2);
	e = sqrt(buf);
	std::cout << "CheckSpectralCriterial: " << "e = " << e.real() << std::endl;
}
void StartCheckSpectralCriterial(std::string const& bitSequence)
{
	const int16_t MAX_N = 100;


	int16_t u[MAX_N];
	size_t N = bitSequence.size();
	std::complex<double> p[MAX_N];

	//u
	for (int16_t i = 0; i < N; i++) u[i] = (bitSequence[i] == '1') ? -1 : 1;

	//p
	for (int16_t m = 0; m < N; m++)
	{
		p[m] = 0;
		for (int16_t n = 0; n < N; n++)
		{
			p[m] += std::complex<double>(u[n]) * std::complex<double>(cos(-2 * M_PI * m * n / N), sin(-2 * M_PI * m * n / N));
		}
	}
	PrintCheckExpCriterial(N, p);

	//e
	PrintCheckSpectralCriterial(N, p);
}