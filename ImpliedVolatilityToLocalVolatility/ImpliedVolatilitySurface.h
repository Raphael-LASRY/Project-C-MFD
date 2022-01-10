#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

using Vector = vector<double>;
using Matrix = vector<Vector>;

class ImpliedVolatilitySurface
{
public:
	// Constructor
	ImpliedVolatilitySurface(const Vector& maturities, const Vector& strikes, const Matrix& implied_vols, const double& risk_free_rate, Vector& discretize_strikes, Vector& discretize_maturities);

	// Constructor reading a csv file
	ImpliedVolatilitySurface(const string& filename, const double& risk_free_rate, Vector& discretize_strikes, Vector& discretize_maturities);

	// return sigma*(T,K) for all T, K >= 0
	double implied_volatility(const double& maturity, const double& strike) const;

	// Export the results
	void export_implied_volatility(const string& filename);


private:
	// Evaluate the coefficients
	void evaluate_cubic_spline_coefficients();

	// For a given index {1, ... , M}, compute sigma*(T_i, K) for all K >= 0
	double compute_smile_implied_vol(const size_t& maturity_index, const double& strike) const;

	Matrix _alpha_coefficients; 
	// alpha_1, ..., alpha_{N-1} [T_1]
	// ...............................
	// alpha_1, ...., alpha_{N-1} [T_M]

	Matrix _beta_coefficients;
	Matrix _gamma_coefficients;

	Vector _delta_K;
	Vector _D_L;
	Vector _D_R;
	
	Vector _maturities; // [T_1, ..., T_M]
	Vector _strikes; // [K_1, ..., K_N]
	Matrix _market_implied_vols; // Matrix M*N 
	// [sigma*_1,1, ... sigma*_1,N]
	// ..........................
	// ...........................
	// [sigma*_M,1, ... sigma*_M,N]

	Vector _discretize_maturities;
	Vector _discretize_strikes;

	double _risk_free_rate; // We could have used any time-dependent function.
};