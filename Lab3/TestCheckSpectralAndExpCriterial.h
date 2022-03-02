#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <complex>
#include <iomanip>

void PrintCheckExpCriterial(size_t N, std::complex<double>* p);
void PrintCheckSpectralCriterial(size_t N, std::complex<double>* p);
void StartCheckSpectralCriterial(std::string const& bitSequence);