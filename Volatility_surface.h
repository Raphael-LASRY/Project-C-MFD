#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <stdlib.h> 
#include <iomanip>

class Volatility_surface{

public :

	double RISK_FREE_RATE = 0.05;

	std::vector<double> maturities;
	std::vector<double> strikes;
	Matrix implied_volatility_bs;


	int nb_disc_maturity;             // Number of points of discretization for maturity
	int nb_disc_strike;               // Number of points of discretization for strike


	std::vector<double> discretize_maturities;
	std::vector<double> discretize_strikes;

	Matrix volatility_surface_discretize;    // Matrix of the volatility surface 


	// Constructor with parameters 
	Volatility_surface(const std::vector<double> maturities_input, const std::vector<double> strikes_input,
		const Matrix implied_volatility, const std::vector<double> discretize_maturities_input, const std::vector<double> discretize_strikes_input);

	// Constructor with a csv 
	Volatility_surface(const std::string filename, const std::vector<double> discretize_maturities_input, const std::vector<double> discretize_strikes_input);

	// Destructor
	virtual ~Volatility_surface() = default;


	void partial_fill_volatility_surface(const int idx1, const int idx2);   // idx1 and idx2 are the index of maturities T1 and T2

	void fill_volatility_surface();

};
