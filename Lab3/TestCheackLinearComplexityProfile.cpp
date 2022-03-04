#include "TestCheackLinearComplexityProfile.h"

void StartCheackLinearComplexityProfile(std::string key)
{
	const int16_t max_N = 100;
	int16_t s[max_N], c[max_N], b[max_N], t[max_N], buf[max_N], l, m, d;
	int16_t N = key.size();

	for (int16_t i = 0; i < N; i++)
	{
		if (key[i] == '1') s[i] = 1;
		else s[i] = 0;
	}

	for (int16_t i = 0; i <= N; i++)
	{
		b[i] = 0;
		c[i] = 0;
		t[i] = 0;
	}
	b[0] = c[0] = 1;
	l = 0;
	m = -1;
	std::cout << "StartCheackLinearComplexityProfile: " << std::endl;
	for (int16_t n = 0; n < N; n++)
	{
		d = s[n];
		for (int16_t i = 1; i <= l; i++) d += (c[i] * s[n - i]);
		d = d % 2;
		for (int16_t i = 0; i < N; i++) buf[i] = 0;
		if (d == 1)
		{
			for (int16_t i = 0; i <= l; i++) t[i] = c[i];
			for (int16_t i = 0; i <= n - m; i++) buf[i + n - m] = b[i];
			for (int16_t i = 0; i < N; i++) c[i] = (c[i] + buf[i]) % 2;
			if (l <= n / 2)
			{
				l = n + 1 - l;
				m = n;
				for (int16_t i = 0; i <= l; i++) b[i] = t[i];
			}
		}
		std::cout << "L= " << l << "\t Sn: ";
		for (int16_t i = 0; i <= n; i++)
		{
			std::cout << s[i];
			if (i != n) std::cout << ", ";
		}
		std::cout << std::endl;
	}
}
