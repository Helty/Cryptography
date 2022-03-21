#include "TestCheackLinearComplexityProfile.h"

void StartCheackLinearComplexityProfile(std::string const& bitSequence)
{
	int64_t l, m, d = 0;
	std::vector<int64_t> s, c, b, t, buf;
	int64_t N = bitSequence.size();

	for (int64_t i = 0; i < N; i++)
	{
		s.push_back((bitSequence[i] == '1') ? 1 : 0);
	}

	for (int64_t i = 0; i <= N; i++)
	{
		b.push_back(0);
		c.push_back(0);
		t.push_back(0);
	}
	b[0] = c[0] = 1;
	l = 0;
	m = -1;
	std::cout << "StartCheackLinearComplexityProfile: " << std::endl;
	for (int64_t n = 0; n < N; n++)
	{
		d = s[n];
		for (int64_t i = 1; i <= l; i++) d += (c[i] * s[n - i]);
		d = d % 2;
		buf.clear();
		for (int64_t i = 0; i < N; i++) buf.push_back(0);
		if (d == 1)
		{
			for (int64_t i = 0; i <= l; i++) t[i] = c[i];
			for (int64_t i = 0; i <= n - m; i++) buf[i + n - m] = b[i];
			for (int64_t i = 0; i < N; i++) c[i] = (c[i] + buf[i]) % 2;
			if (l <= n / 2)
			{
				l = n + 1 - l;
				m = n;
				for (int64_t i = 0; i <= l; i++) b[i] = t[i];
			}
		}
		/*std::cout << "\tL= " << l << "\t Sn: ";
		for (int64_t i = 0; i <= n; i++)
		{
			std::cout << s[i];
			if (i != n) std::cout << ", ";
		}
		std::cout << std::endl;*/
	}
	std::cout << "\tL= " << l << std::endl;
}
