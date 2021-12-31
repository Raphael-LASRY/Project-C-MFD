#pragma once
#include <cmath>
#include <vector>
#include "Matrix.h"

void display_vector(const std::vector<double> v);

std::vector<double> linspace(double min_val, double max_val, int nb_disc);

int left_index(const double val, std::vector<double> vect);

Matrix compute_cubic_spline_interpolation(const std::vector<double> strikes, const std::vector<double> implied_volatilities);

double cubic_polynomial(const double& shift, std::vector<double> coeff, const double& x);