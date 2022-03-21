#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <complex>
#include <vector>
#include <iomanip>

void PrintCheckExpCriterial(int64_t N, std::vector<std::complex<double>> const& p);

void PrintCheckSpectralCriterial(int64_t N, std::vector<std::complex<double>> const& p);

void StartCheckSpectralAndExpCriterial(std::string const& bitSequence);