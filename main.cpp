#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include "math.h"
#include "matrix.h"
#include "volatility_surface.h"
#include "utils.h"

int main()
{
	string filename = "Implied_Volatilities_Example.csv";

	std::vector<double> discretize_maturities = arange(0.05, 6, 0.05);
	std::vector<double> discretize_strikes = arange(10, 200, 10);

	Volatility_surface vol_surface(filename, discretize_maturities, discretize_strikes);

	vol_surface.fill_volatility_surface();

	cout << vol_surface._volatility_surface_discretize << endl;
	export_volatility_surface_discretize("Results.csv", vol_surface._volatility_surface_discretize, discretize_maturities, discretize_strikes);
	return 0;
}