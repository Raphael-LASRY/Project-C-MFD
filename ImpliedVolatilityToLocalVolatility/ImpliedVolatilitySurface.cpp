#include "ImpliedVolatilitySurface.h"
#include "ThomasSolver.h"


// Initialization List !!!
ImpliedVolatilitySurface::ImpliedVolatilitySurface(const Vector& maturities, const Vector& strikes, const Matrix& implied_vols, const double& risk_free_rate, Vector& discretize_strikes, Vector& discretize_maturities)
	: _maturities(maturities), _strikes(strikes), _market_implied_vols(implied_vols), _risk_free_rate(risk_free_rate), _discretize_strikes(discretize_strikes), _discretize_maturities(discretize_maturities)
{
	// _delta_K[j] = K_{j+1} - K_j
	for (size_t j = 0; j < _strikes.size() - 1; ++j)
		_delta_K.push_back(_strikes[j + 1] - _strikes[j]);

	evaluate_cubic_spline_coefficients();
}

ImpliedVolatilitySurface::ImpliedVolatilitySurface(const string& filename, const double& risk_free_rate, Vector& discretize_strikes, Vector& discretize_maturities)
	: _risk_free_rate(risk_free_rate), _discretize_strikes(discretize_strikes), _discretize_maturities(discretize_maturities)
{
	Vector maturities;
	Vector strikes;

	// Create an input filestream
	ifstream csv_file(filename);

	// Make sure the file is open
	if (!csv_file.is_open()) throw runtime_error("Could not open file");

	// Helper vars
	string line;
	string strike;

	// Read the column names
	if (csv_file.good())
	{
		// Extract the first line in the file
		getline(csv_file, line);

		// Create a stringstream from line
		stringstream ss(line);
		int col_idx = 0;

		while (getline(ss, strike, ','))
		{
			if (col_idx > 0)
				strikes.push_back(stod(strike));
			col_idx++;
		};
	}
	Matrix csv_rows;

	// Read data line by line 
	while (getline(csv_file, line))
	{
		// Create a stringstream of the current line 
		stringstream ss(line);

		// Keep track of current column index 
		int col_idx = 0;

		Vector current_row;

		string val;

		while (getline(ss, val, ','))
		{
			if (col_idx > 0)
				current_row.push_back(stod(val));
			else
				maturities.push_back(stod(val));
			col_idx++;
		}
		csv_rows.push_back(current_row);
	};

	int n_rows = maturities.size();
	int n_cols = strikes.size();


	for (int row_idx = 0; row_idx < n_rows; row_idx++)
	{
		Vector _market_implied_vols_row;
		for (int col_idx = 0; col_idx < n_cols; col_idx++)
			_market_implied_vols_row.push_back(csv_rows[row_idx][col_idx]);
		_market_implied_vols.push_back(_market_implied_vols_row);
	}
	_maturities = maturities;
	_strikes = strikes;

	// _delta_K[j] = K_{j+1} - K_j
	for (size_t j = 0; j < _strikes.size() - 1; ++j)
		_delta_K.push_back(_strikes[j + 1] - _strikes[j]);

	evaluate_cubic_spline_coefficients();
}


void ImpliedVolatilitySurface::evaluate_cubic_spline_coefficients()
{
	for (size_t mat_idx = 0; mat_idx < _maturities.size(); ++mat_idx)
	{
		Vector lower_diag;
		Vector central_diag;
		Vector upper_diag;
		Vector rhs;

		size_t strike_size = _strikes.size();

		for (size_t j = 0; j < strike_size - 3; ++j)
		{
			lower_diag.push_back(_delta_K[j + 1]);
			upper_diag.push_back(_delta_K[j + 1]);
		}

		Vector Y;
		for (size_t j = 0; j < strike_size; ++j)
			Y.push_back(_market_implied_vols[mat_idx][j]);

		for (size_t j = 0; j < strike_size - 2; ++j)
		{
			central_diag.push_back(2. * (_delta_K[j] + _delta_K[j + 1]));
			rhs.push_back(3. * ((Y[j + 2] - Y[j + 1]) / _delta_K[j + 1] - (Y[j + 1] - Y[j]) / _delta_K[j]));
		}

		ThomasSolver thomas_solver(lower_diag, central_diag, upper_diag, rhs);
		Vector solution = thomas_solver.solve();

		Vector beta;
		beta.push_back(0.);
		for (size_t j = 1; j < strike_size - 1; ++j)
			beta.push_back(solution[j - 1]);

		_beta_coefficients.push_back(beta);

		Vector alpha; // maturity T[mat_idx]
		for (size_t j = 0; j < strike_size - 2; ++j)
			alpha.push_back((beta[j + 1] - beta[j]) / (3 * _delta_K[j]));
		alpha.push_back(-beta[strike_size - 2] / (3 * _delta_K[strike_size - 2]));
		_alpha_coefficients.push_back(alpha);

		Vector gamma;
		for (size_t j = 0; j < strike_size - 1; ++j)
			gamma.push_back((Y[j + 1] - Y[j]) / _delta_K[j] - alpha[j] * pow(_delta_K[j], 2) - beta[j] * _delta_K[j]);
		_gamma_coefficients.push_back(gamma);

		_D_L.push_back(gamma[0]);
		_D_R.push_back(3 * alpha[strike_size - 2] * pow(_delta_K[strike_size - 2], 2) + 2 * beta[strike_size - 2] * _delta_K[strike_size - 2] + gamma[strike_size - 2]);
	}
}

double ImpliedVolatilitySurface::compute_smile_implied_vol(const size_t& maturity_index, const double& strike) const
{
	if (strike <= _strikes[0])
		return _market_implied_vols[maturity_index][0] + _D_L[maturity_index] * (strike - _strikes[0]);
	else if (strike >= _strikes[_strikes.size() - 1])
		return _market_implied_vols[maturity_index][_strikes.size() - 1] + _D_R[maturity_index] * (strike - _strikes[_strikes.size() - 1]);
	else
	{
		int strike_index = 0;
		while (_strikes[strike_index] <= strike)
			++strike_index;
		--strike_index;

		return _alpha_coefficients[maturity_index][strike_index] * pow(strike - _strikes[strike_index], 3) + _beta_coefficients[maturity_index][strike_index] * pow(strike - _strikes[strike_index], 2) + _gamma_coefficients[maturity_index][strike_index] * (strike - _strikes[strike_index]) + _market_implied_vols[maturity_index][strike_index];
	}
}

double ImpliedVolatilitySurface::implied_volatility(const double& maturity, const double& strike) const
{
	if (maturity <= _maturities[0])
		return compute_smile_implied_vol(0, strike * exp(_risk_free_rate * (_maturities[0] - maturity)));
	else if (maturity >= _maturities[_maturities.size() - 1])
		return compute_smile_implied_vol(_maturities.size() - 1, strike * exp(_risk_free_rate * (_maturities[_maturities.size() - 1] - maturity)));
	else
	{
		int maturity_index = 0;
		while (_maturities[maturity_index] <= maturity)
			maturity_index++;
		maturity_index--;

		double k_F = strike * exp(- _risk_free_rate * maturity);
		double K_i = k_F * exp(_risk_free_rate * _maturities[maturity_index]);
		double K_i_1 = k_F * exp(_risk_free_rate * _maturities[maturity_index + 1]);

		double v_T_i_k = pow(compute_smile_implied_vol(maturity_index, K_i), 2) * _maturities[maturity_index];
		double v_T_i_k_f = (pow(compute_smile_implied_vol(maturity_index + 1, K_i_1), 2) * _maturities[maturity_index + 1] - v_T_i_k) * (maturity - _maturities[maturity_index]) / (_maturities[maturity_index + 1] - _maturities[maturity_index]);

		return sqrt((v_T_i_k + v_T_i_k_f) / maturity);
	}
}

void ImpliedVolatilitySurface::export_implied_volatility(const string& filename)
{
	ofstream file;
	file.open(filename);
	int n_rows = _discretize_maturities.size();
	int n_cols = _discretize_strikes.size();
	for (int col_idx = 0; col_idx < n_cols; col_idx++)
		file << ";";
	file << ";" << "\n";
	file << ";Maturities / Strikes;";
	for (int col_idx = 0; col_idx < n_cols; col_idx++)
		file << _discretize_strikes[col_idx] << ";";
	file << "\n";
	for (int row_idx = 0; row_idx < n_rows; row_idx++)
	{
		file << ";" << _discretize_maturities[row_idx] << ";";
		for (int col_idx = 0; col_idx < n_cols; col_idx++)
			file << implied_volatility(_discretize_maturities[row_idx], _discretize_strikes[col_idx]) << ";";
		file << "\n";
	}
	file.close();
}