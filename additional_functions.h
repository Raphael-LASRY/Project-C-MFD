#pragma once
#include <cmath>
#include <vector>
#include "Matrix.h"

void display_vector(const std::vector<double> v);

Matrix compute_cubic_spline_interpolation(const std::vector<double> strikes, const std::vector<double> implied_volatilities);

double cubic_polynomial(const double& shift, std::vector<double> coeff, const double& x);