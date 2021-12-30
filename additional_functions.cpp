#include "additional_functions.h"
#include "Matrix.h"


void display_vector(const vector<double> v)
{
	int n = v.size();
	for (int i = 0; i < n; i++) 
	{
		cout << v[i] << " ";
	};
	cout << endl; 
}


Matrix compute_cubic_spline_interpolation(const vector<double> strikes, const vector<double> implied_volatilities) 
{
	int nb_rows = 4;
	int nb_cols = strikes.size() - 1;
	Matrix M = Matrix(nb_rows, nb_cols); // M = (alpha_j, beta_j, gamma_j, delta_j)

	vector<double> delta_strikes;
	for (int i = 0; i < strikes.size() - 1; i++)
		delta_strikes.push_back(strikes[i + 1] - strikes[i]);

	// Delta
	for (int col_idx = 0; col_idx < nb_cols; col_idx++)
		M(3, col_idx) = implied_volatilities[col_idx];

	// Beta
	double a, b, c, R;
	vector<double> c_vect, R_vect;
	double c_bis = 0;
	double R_bis = 0;
	for (int col_idx = 0; col_idx < nb_cols - 1; col_idx++)
	{
		a = delta_strikes[col_idx];
		b = 2 * (delta_strikes[col_idx] + delta_strikes[col_idx + 1]);
		c = delta_strikes[col_idx];
		R = 3 * ((implied_volatilities[col_idx + 2] - implied_volatilities[col_idx + 1]) / delta_strikes[col_idx + 1] -
			(implied_volatilities[col_idx + 1] - implied_volatilities[col_idx]) / delta_strikes[col_idx]);
		c_vect.push_back(c / (b - a * c_bis));
		R_vect.push_back((R - a * R_bis) / (b - a * c_bis));
		c_bis = c_vect[col_idx];
		R_bis = R_vect[col_idx];
	}
	M(1, nb_cols - 1) = R_bis;
	for (int col_idx = nb_cols - 2; col_idx > 0; col_idx--)
	{
		M(1, col_idx) = R_vect[col_idx] - c_vect[col_idx] * M(1, col_idx + 1);
	}

	// Alpha 
	for (int col_idx = 0; col_idx < nb_cols - 2; col_idx++)
	{
		M(0, col_idx) = (M(1, col_idx + 1) - M(1, col_idx)) / (3 * delta_strikes[col_idx]);
	}
	M(0, nb_cols - 1) = -M(1, nb_cols - 1) / (3 * delta_strikes[nb_cols - 1]);

	// Gamma
	for (int col_idx = 0; col_idx < nb_cols - 1; col_idx++)
		M(2, col_idx) = (implied_volatilities[col_idx + 1] - implied_volatilities[col_idx]) / delta_strikes[col_idx] - 
						M(0, col_idx) * delta_strikes[col_idx] * delta_strikes[col_idx] - 
						M(1, col_idx) * delta_strikes[col_idx];
	return M;
};


double cubic_polynomial(const double& shift, vector<double> coeff, const double& x) 
{
	double res = 0;
	int size = coeff.size();
	for (int i = 0; i < size; i++) {
		res += coeff[i] * pow(x - shift, i);
	};
	return res;
};
