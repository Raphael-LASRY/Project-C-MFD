#pragma once
#include "matrix.h"
#include "utils.h"

class Volatility_surface
{
protected:
	double _risk_free_rate = 0.05;

	vector<double> _maturities;
	vector<double> _strikes;
	Matrix _implied_volatility_bs;

	vector<double> _discretize_maturities;
	vector<double> _discretize_strikes;

public:

	Matrix _volatility_surface_discretize; // Matrix of the volatility surface 

	// Constructor with parameters 
	Volatility_surface(const vector<double>& maturities, const vector<double>& strikes,
		const Matrix& implied_volatility, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes);

	// Constructor with a csv 
	Volatility_surface(const string& filename, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes);

	// Destructor
	virtual ~Volatility_surface() = default;

	void partial_fill_volatility_surface(const int& idx1, const int& idx2);

	void fill_volatility_surface();
};
