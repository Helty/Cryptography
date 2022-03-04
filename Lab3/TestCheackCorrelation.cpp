#include "TestCheackCorrelation.h"

void StartCheackCorrelation(std::string bitSequence)
{
	const int16_t max_N = 100;
	int16_t a[max_N], u[max_N], r[max_N], rr[max_N];
	int16_t N = bitSequence.size();
	for (int16_t i = 0; i < N; i++)
	{
		if (bitSequence[i] == '1')
		{
			a[i] = 1;
			u[i] = -1;
		}
		else
		{
			a[i] = 0;
			u[i] = 1;
		}
	}

	//смещение t
	int16_t t, g;
	for (int16_t i = 0; i < N; i++)
	{
		t = 0;
		for (int16_t j = 0; j < N - i; j++)
		{
			rr[t] = (a[j] + a[i + j]) % 2;
			t++;
		}

		g = 0;
		for (int16_t j = 0; j < t; j++)
		{
			if (rr[j] == 1)	g++;
		}
		r[i] = N - i - 2 * g;
	}

	int16_t max_r = 0, sq = 0;
	for (int16_t i = 1; i < N; i++)
	{
		// max_r
		if (max_r < abs(r[i])) max_r = abs(r[i]);

		// сумма квадратов элементов r
		sq += pow(r[i], 2);
	}

	std::cout << "CheackCorrelation: ";

	float R = N / max_r;
	std::cout << "R= " << R;

	float MF = N / sqrt(sq);
	std::cout << " MF= " << MF << std::endl;
}