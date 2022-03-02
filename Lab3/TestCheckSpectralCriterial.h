#pragma once
#include <cmath>
#include <iostream>
#include <complex>
#include <iomanip>

void RrintUInConsole(uint16_t N, uint16_t* u);
void RrintPInConsole(uint16_t N, std::complex<double>* p);
void PrintEInConsole(uint16_t N, std::complex<double>* p);
void StartCheckSpectralCriterial(std::string const& bitSequence);