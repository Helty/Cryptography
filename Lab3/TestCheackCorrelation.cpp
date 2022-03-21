#include "TestCheackCorrelation.h"

void StartCheackCorrelation(std::string const& bitSequence)
{
	std::vector<int64_t> a, u, r, rr;
	int64_t N = bitSequence.size();

	for (int64_t i = 0; i < N; i++)
	{
		if (bitSequence[i] == '1')
		{
			a.push_back(1);
			u.push_back(-1);
		}
		else
		{
			a.push_back(0);
			u.push_back(1);
		}
	}

	//смещение t
	int64_t t, g;
	for (int64_t i = 0; i < N; i++)
	{
		t = 0;
		rr.clear();
		for (int64_t j = 0; j < N - i; j++)
		{
			rr.push_back((a[j] + a[i + j]) % 2);
			t++;
		}

		g = 0;
		for (int64_t j = 0; j < t; j++)
		{
			if (rr[j] == 1)	g++;
		}
		r.push_back((N - i - 2 * g));
	}

	double max_r = 0, sq = 0;
	for (int64_t i = 1; i < N; i++)
	{
		// max_r
		if (max_r < abs(r[i])) max_r = static_cast<double>(abs(r[i]));

		// сумма квадратов элементов r
		sq += pow(r[i], 2);
	}

	std::cout << "CheackCorrelation: " << std::endl;

	double R = (double)(N / max_r);
	std::cout << "\tR= " << R << std::endl;

	double MF = N / sqrt(sq);
	std::cout << "\tMF= " << MF << std::endl;
}