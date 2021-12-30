#include "additional_functions.h"
#include "Matrix.h"


void display_vector(const std::vector<double> v) {
	int n = v.size();
	for (int i = 0; i < n; i++) {
		cout << v[i] << " ";
	};
	cout << endl; 
}


Matrix compute_cubic_spline_interpolation(const std::vector<double> strikes, const std::vector<double> implied_volatilities) {
	int nb_rows = 4;
	int nb_cols = strikes.size() - 1;
	Matrix M = Matrix(nb_rows,nb_cols,0);
	return M;
};


double cubic_polynomial(const double& shift, std::vector<double> coeff, const double& x) {
	double res = 0;
	int size = coeff.size();
	for (int i = 0; i < size; i++) {
		res += coeff[i] * pow(x - shift, i);
	};
	return res;
};
