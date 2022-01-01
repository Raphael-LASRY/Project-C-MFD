#include "utils.h"
#include "matrix.h"


vector<double> linspace(const double& start, const double& stop, const int& num)
{
	vector<double> res;
	double step = (stop - start) / num;
	for (int i = 0 ; i <= num; i++)
		res.push_back(start + i * step);
	return res;
}

vector<double> arange(const double& start, const double& stop, const double& step)
{
	vector<double> res;
	for (double idx = start; idx <= stop; idx += step)
		res.push_back(idx);
	return res;
}

int left_index(const double& val, const vector<double>& vect) // Vect is assumed to be sorted
{
	assert(is_sorted(vect.begin(), vect.end()));
	int res = -1;   // If val is lower than the lower bound of vect, we return -1
	int length = vect.size();
	for (int idx = 0; idx < length; idx++)
	{
		if (val <= vect[idx])
			return idx;
		res = idx;
	}
	return res;
}

ostream& operator<<(ostream& str, const vector<double>& vect)
{
	for (int idx = 0; idx < vect.size(); idx++)
		str << setiosflags(ios::fixed) << setprecision(1) << scientific << vect[idx] << ' ';
	str << endl;
	return str;
}

Matrix compute_cubic_spline_interpolation(const vector<double>& strikes, const vector<double>& implied_volatilities)
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
}


double cubic_polynomial(const double& shift, const vector<double>& coeff, const double& x) 
{
	double res = 0;
	int size = coeff.size();
	for (int i = 0; i < size; i++)
		res += coeff[i] * pow(x - shift, size-i-1);     // The first value of coeff is the highest order coefficient
	return res;
}

void export_volatility_surface_discretize(const string& filename, const Matrix& volatility_surface_discretize, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes)
{
	ofstream file;
	file.open(filename);
	int n_rows = discretize_maturities.size();
	int n_cols = discretize_strikes.size();
	file << "Maturities / Strikes;";
	for (int col_idx = 0; col_idx < n_cols; col_idx++)
		file << discretize_strikes[col_idx] << ";";
	file << "\n";
	for (int row_idx = 0; row_idx < n_rows; row_idx++)
	{
		file << discretize_maturities[row_idx] << ";";
		for (int col_idx = 0; col_idx < n_cols; col_idx++)
		{
			file << volatility_surface_discretize(row_idx, col_idx) << ";";
		}
		file << "\n";
	}
	file.close();
}
