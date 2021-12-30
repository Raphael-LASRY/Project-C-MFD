#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <stdlib.h> 
#include <iomanip>

class Volatility_surface{

public :

	double RISK_FREE_RATE = 0.02;

	std::vector<double> maturities;
	std::vector<double> strikes;
	Matrix implied_volatility_bs;


	int step_maturity;             // Number of points we want between two market maturities
	int step_strike;               // Number of points we want between two market strikes

	std::vector<std::vector<std::vector<double>>> polynomial_coefficients;

	std::vector<double> discretize_maturities;
	std::vector<double> discretize_strikes;
	Matrix volatility_surface_discretize;    // Matrix of the volatility surface 


	// Constructor with parameters 
	Volatility_surface(const std::vector<double> maturities_input, const std::vector<double> strikes_input,
		const Matrix implied_volatility, const int step_maturity_input, const int step_strike_input);

	// Constructor with a csv 
	Volatility_surface(const std::string filename, const int step_maturity_input, const int step_strike_input);

	// Destructor
	virtual ~Volatility_surface() = default;


	void partial_fill_volatility_surface(const int idx1, const int idx2);   // idx1 and idx2 are the index of maturities T1 and T2

	void fill_volatility_surface();

};
