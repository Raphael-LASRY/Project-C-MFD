#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <stdlib.h> 
#include "Matrix.h"
#include <iomanip>

using namespace std;

class Volatility_surface{

protected : 

	std::vector<double> maturities;
	std::vector<double> strikes;
	Matrix  implied_volatility_bs;


	int disc_strike;
	int disc_maturities;

	Matrix polynomial_coefficients; 

	Matrix  volatility_surface;

public : 

	// Constructor with parameters 
	Volatility_surface(const std::vector<double> maturities, std::vector<double> strikes, Matrix implied_volatility);

	// Destructor
	virtual ~Volatility_surface() = default;


	void compute_polynomial_coefficients(); 

	void fill_volatility_surface();

};
