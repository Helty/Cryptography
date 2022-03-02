#pragma once
#include <cmath>
#include <iostream>
#include <complex>
#include <iomanip>

void PrintCheckExpCriterial(int16_t N, std::complex<double>* p);
void PrintCheckSpectralCriterial(int16_t N, std::complex<double>* p);
void StartCheckSpectralCriterial(std::string const& bitSequence);