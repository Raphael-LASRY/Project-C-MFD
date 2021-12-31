#pragma once
#include "volatility_surface.h"

Volatility_surface::Volatility_surface(const vector<double>& maturities, const vector<double>& strikes,
	const Matrix& implied_volatility, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes)
{
	_maturities = maturities;
	_strikes = strikes;
	_implied_volatility_bs = implied_volatility;
	_discretize_maturities = discretize_maturities;
	_discretize_strikes = discretize_strikes;
	_volatility_surface_discretize = Matrix(_discretize_maturities.size(), _discretize_strikes.size());
}

Volatility_surface::Volatility_surface(const string& filename, const vector<double>& discretize_maturities, const vector<double>& discretize_strikes)
{

	vector<double> maturities;
	vector<double> strikes;

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
	vector<vector<double>> csv_rows;

	// Read data line by line 
	while (getline(csv_file, line))
	{
		// Create a stringstream of the current line 
		stringstream ss(line);

		// Keep track of current column index 
		int col_idx = 0;

		vector<double> current_row;

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


	_implied_volatility_bs = Matrix(n_rows, n_cols);
	for (int row_idx = 0; row_idx < n_rows; row_idx++)
	{
		for (int col_idx = 0; col_idx < n_cols; col_idx++)
			_implied_volatility_bs(row_idx, col_idx) = csv_rows[row_idx][col_idx];
	}
	_maturities = maturities;
	_strikes = strikes;
	_discretize_maturities = discretize_maturities;
	_discretize_strikes = discretize_strikes;
	_volatility_surface_discretize = Matrix(_discretize_maturities.size(), _discretize_strikes.size());
}

void Volatility_surface::partial_fill_volatility_surface(const int& idx1, const int& idx2)
{
	// Get Matrix rows at idx1 and idx2 : 
	if (_discretize_maturities[idx1] > _maturities[0])
	{
		// TODO : SI T < T_1 OU T > T_M
	}
	double maturity_1 = _maturities[idx1];
	double maturity_2 = _maturities[idx2];

	vector<double> implied_vol_1, implied_vol_2; 
	for (int col_idx = 0; col_idx < _strikes.size(); col_idx++)
	{
		implied_vol_1.push_back(_implied_volatility_bs(idx1, col_idx));
		implied_vol_2.push_back(_implied_volatility_bs(idx2, col_idx));
	}
	Matrix coeff1 = compute_cubic_spline_interpolation(_strikes, implied_vol_1);
	Matrix coeff2 = compute_cubic_spline_interpolation(_strikes, implied_vol_2); 

	// We determine all indexes of discretize_maturities corresponding of maturities between T_1 and T_2 : 
	vector<int> maturity_indexes;
	vector<int> maturity_indexes_bis;
	for (int i = 0; i < _discretize_maturities.size(); i++) 
	{
		if (_discretize_maturities[i] >= maturity_1 and _discretize_maturities[i] <= maturity_2)
			maturity_indexes.push_back(i);
		else
			maturity_indexes_bis.push_back(i);
	}
	for (int i = 0; i < maturity_indexes.size(); i++) // T^(1) <= T_1 < T_2 <= T^(M)
	{
		int maturity_idx = maturity_indexes[i];
		double maturity = _discretize_maturities[maturity_idx]; 
		for (int j = 0; j < _discretize_strikes.size(); j++)
		{
			double strike = _discretize_strikes[j]; 
			double K_1 = strike * exp((maturity - maturity_1) * _risk_free_rate);
			double K_2 = strike * exp((maturity_2 - maturity) * _risk_free_rate);

			int left_index_1 = left_index(K_1, _strikes); 
			int left_index_2 = left_index(K_2, _strikes);

			double sigma_1; 
			double sigma_2;

			int strikes_length = _strikes.size(); 

			if (left_index_1 == -1) // K_1 < K^(1)
			{
				vector<double> coeff = coeff1.get_col(0); 
				double D_L = coeff[2]; 
				sigma_1 = _implied_volatility_bs(idx1, 0) + D_L * (strike - _strikes[0]);
			}
			else if (left_index_1 == (strikes_length - 1)) // K_1 > K^(M)
			{
				vector<double> coeff = coeff1.get_col(strikes_length - 2);
				double delta_strikes = _strikes[strikes_length - 1] - _strikes[strikes_length - 2];
				double D_R = 3 * coeff[0] * pow(delta_strikes, 2) + 2 * coeff[1] * delta_strikes + coeff[2] ;
				sigma_1 = _implied_volatility_bs(idx1, strikes_length - 1) + D_R * (strike - _strikes[strikes_length - 1]);
			}
			else // K^(1) <= K_1 <= K^(M)
			{
				vector<double> coeff = coeff1.get_col(left_index_1); 
				double left_strike = _strikes[left_index_1]; 
				sigma_1 = cubic_polynomial(left_strike, coeff, strike); 
			}

			if (left_index_2 == -1) // K_2 < K^(1) 
			{
				vector<double> coeff = coeff2.get_col(0);
				double D_L = coeff[2];
				sigma_2 = _implied_volatility_bs(idx2, 0) + D_L * (strike - _strikes[0]);
			}
			else if (left_index_2 == (strikes_length - 1)) // K_2 > K^(M)
			{
				vector<double> coeff = coeff2.get_col(strikes_length - 2);
				double delta_strikes = _strikes[strikes_length - 1] - _strikes[strikes_length - 2];
				double D_R = 3 * coeff[0] * pow(delta_strikes, 2) + 2 * coeff[1] * delta_strikes + coeff[2];
				sigma_2 = _implied_volatility_bs(idx2, strikes_length - 1) + D_R * (strike - _strikes[strikes_length - 1]);
			}
			else // K^(1) <= K_2 <= K^(M)
			{
				vector<double> coeff = coeff2.get_col(left_index_2);
				double left_strike = _strikes[left_index_2];
				sigma_2 = cubic_polynomial(left_strike, coeff, strike);
			}
			double delta_square = ((pow(sigma_2, 2) * maturity_2 - pow(sigma_1, 2) * maturity_1) / (maturity_2 - maturity_1));
			_volatility_surface_discretize(maturity_idx, j) = sqrt((1 / maturity) * (pow(sigma_1, 2) * maturity_1 + delta_square * (maturity - maturity_1))); 
		}
	}

}

void Volatility_surface::fill_volatility_surface() 
{
	for (int i = 0; i < _discretize_maturities.size() - 1; i++)
		this->partial_fill_volatility_surface(i, i + 1);
}
