#include "TestCheackLinearComplexityProfile.h"

void StartCheackLinearComplexityProfile(std::string const& bitSequence)
{
	int64_t n = bitSequence.size();
	std::vector<int64_t> s;
	for (int64_t i = 0; i < n; i++)
	{
		s.push_back((bitSequence[i] == '1') ? 1 : 0);
	}

	std::vector<int64_t> c(n, 0); c[0] = 1;
	std::vector<int64_t> b(n, 0); b[0] = 1;
	int64_t L = 0, m = -1, N = 0;

	while (N < n)
	{
		int64_t sum = 0;
		for (size_t i = 1; i < L + 1; i++)
		{
			sum = (sum + c[i] * s[N - i]) % 2;
		}
		int64_t d = (s[N] + sum) % 2;
		if (d == 1)
		{
			std::vector<int64_t> t = c;

			for (size_t j = 0; j < (n - N + m); j++)
			{
				c[N - m + j] = (c[N - m + j] + b[j]) % 2;
			}

			if (L <= N / 2)
			{
				L = N + 1 - L;
				m = N;
				b = t;
			}
		}
		N += 1;
	}
	std::cout << "StartCheackLinearComplexityProfile: " << std::endl;

	std::cout << "\tL= " << L << std::endl;
}
