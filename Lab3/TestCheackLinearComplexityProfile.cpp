#include "TestCheackLinearComplexityProfile.h"

void StartCheackLinearComplexityProfile(std::string key)
{
	int64_t l, m, d = 0;
	std::vector<int16_t> s, c, b, t, buf;
	size_t N = key.size();

	for (size_t i = 0; i < N; i++)
	{
		s.push_back((key[i] == '1') ? 1 : 0);
	}

	for (size_t i = 0; i <= N; i++)
	{
		b.push_back(0);
		c.push_back(0);
		t.push_back(0);
	}
	b[0] = c[0] = 1;
	l = 0;
	m = -1;
	std::cout << "StartCheackLinearComplexityProfile: " << std::endl;
	for (size_t n = 0; n < N; n++)
	{
		d = s[n];
		for (size_t i = 1; i <= l; i++) d += (c[i] * s[n - i]);
		d = d % 2;
		buf.clear();
		for (size_t i = 0; i < N; i++) buf.push_back(0);
		if (d == 1)
		{
			for (size_t i = 0; i <= l; i++) t[i] = c[i];
			for (size_t i = 0; i <= n - m; i++) buf[i + n - m] = b[i];
			for (size_t i = 0; i < N; i++) c[i] = (c[i] + buf[i]) % 2;
			if (l <= n / 2)
			{
				l = n + 1 - l;
				m = n;
				for (size_t i = 0; i <= l; i++) b[i] = t[i];
			}
		}
		std::cout << "\tL= " << l << "\t Sn: ";
		for (size_t i = 0; i <= n; i++)
		{
			std::cout << s[i];
			if (i != n) std::cout << ", ";
		}
		std::cout << std::endl;
	}
}
