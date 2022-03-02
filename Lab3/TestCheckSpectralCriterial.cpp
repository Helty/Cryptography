#include <cmath>
#include <iostream>
#include <complex>
#include <iomanip>

void RrintUInConsole(uint16_t N, uint16_t* u)
{
	std::cout << std::endl;
	std::cout << "n= " << N << "\tu = {";
	for (uint16_t i = 0; i < N; i++)
	{
		std::cout << u[i];
		if (i != N - 1) std::cout << ",";
	}
	std::cout << "}" << std::endl;
}
void RrintPInConsole(uint16_t N, std::complex<double>* p)
{
	std::cout << "p = {";
	for (uint16_t i = 0; i < N; i++)
	{
		std::cout << std::fixed << std::setprecision(3) << p[i];
		if (i != N - 1) std::cout << ",";
	}
	std::cout << "}" << std::endl;
}
void PrintEInConsole(uint16_t N, std::complex<double>* p)
{
	std::complex<double> e, buf = 0;
	for (uint16_t m = 0; m < N; m++) buf += pow(abs(p[m]) - sqrt(N), 2);
	e = sqrt(buf);
	std::cout << "CheckSpectralCriterial: " << "e = " << e.real() << std::endl; // вывод целой части (число e)
}
void StartCheckSpectralCriterial(std::string const& bitSequence)
{
	const uint16_t MAX_N = 100;
	const double PI = 3.14159265359;

	uint16_t u[MAX_N];
	uint16_t N = static_cast<uint16_t>(bitSequence.size());
	std::complex<double> p[MAX_N];

	//u
	for (uint16_t i = 0; i < N; i++) u[i] = (bitSequence[i] == '1') ? -1 : 1;
	//RrintUInConsole(N, u);

	//p
	for (uint16_t m = 0; m < N; m++)
	{
		p[m] = 0;
		for (uint16_t n = 0; n < N; n++)
		{
			p[m] += std::complex<double>(u[n]) * std::complex<double>(cos(-2 * PI * m * n / N), sin(-2 * PI * m * n / N));
		}
	}
	//RrintPInConsole(N, p);

	//e
	PrintEInConsole(N, p);
}