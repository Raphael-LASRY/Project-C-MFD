#pragma once
#include <algorithm>
#include <fstream>
#include <sstream>
#include "matrix.h"

ostream& operator<<(ostream& str, const vector<double>& vect); // Print a vector

vector<double> linspace(const double& start, const double& stop, const int& num); // Create an equidistributed vector between start and stop with num points
vector<double> arange(const double& start, const double& stop, const double& step); // Create an equidistributed vector between start and stop with step between points

int left_index(const double& val, const vector<double>& vect);

Matrix compute_cubic_spline_interpolation(const vector<double>& strikes, const vector<double>& implied_volatilities);

double cubic_polynomial(const double& shift, const vector<double>& coeff, const double& x);

void export_volatility_surface_discretize(const string& filename, const Matrix& volatility_surface_discretize, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes);
