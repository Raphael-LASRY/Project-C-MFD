#pragma once
#include "Matrix.h"
#include "additional_functions.h"
#include "Volatility_surface.h"


Volatility_surface::Volatility_surface(const std::vector<double> maturities_input, const std::vector<double> strikes_input,
	const Matrix implied_volatility_input, const std::vector<double> discretize_maturities_input, const std::vector<double> discretize_strikes_input) {
	maturities = maturities_input;
	strikes = strikes_input;
	implied_volatility_bs = implied_volatility_input;

	
	discretize_maturities = discretize_maturities_input;
	discretize_strikes = discretize_strikes_input;

	// Create the volatility surface matrix : 
	int nb_disc_maturity = discretize_maturities.size();
	int nb_disc_strike = discretize_strikes.size();

	volatility_surface_discretize = Matrix(nb_disc_maturity, nb_disc_strike, 0); 
	
};


Volatility_surface::Volatility_surface(const std::string filename, const std::vector<double> discretize_maturities_input, const std::vector<double> discretize_strikes_input) {

	std::vector<double> maturities_input;
	std::vector<double> strikes_input;

	// Create an input filestream
	std::ifstream csv_file(filename);

	// Make sure the file is open
	if (!csv_file.is_open()) throw std::runtime_error("Could not open file");

	// Helper vars
	std::string line;
	std::string strike;

	// Read the column names
	if (csv_file.good())
	{
		// Extract the first line in the file
		std::getline(csv_file, line);

		// Create a stringstream from line
		std::stringstream ss(line);
		int colIdx = 0;

		while (std::getline(ss, strike, ',')) {
			if (colIdx > 0) {
				strikes_input.push_back(std::stod(strike));
			};
			colIdx += 1;
		};
	}
	std::vector<std::vector<double>> csv_rows;

	// Read data line by line 
	while (std::getline(csv_file, line)) {

		// Create a stringstream of the current line 
		std::stringstream ss(line);

		// Keep track of current column index 
		int colIdx = 0;

		std::vector<double> current_row;

		std::string val;

		while (std::getline(ss, val, ',')) {
			if (colIdx > 0) {
				current_row.push_back(std::stod(val));
			}
			else {
				maturities_input.push_back(std::stod(val));
			}
			colIdx += 1;
		}
		csv_rows.push_back(current_row);
	};

	int n_rows = maturities_input.size();
	int n_col = strikes_input.size();


	implied_volatility_bs = Matrix(n_rows, n_col, 0);
	for (int i = 0; i < n_rows; i++) {
		for (int j = 0; j < n_col; j++) {
			implied_volatility_bs(i, j) = csv_rows[i][j];
		}
	}
	maturities = maturities_input;
	strikes = strikes_input; 

	discretize_maturities = discretize_maturities_input;
	discretize_strikes = discretize_strikes_input;

	// Create the volatility surface matrix : 
	int nb_disc_maturity = discretize_maturities.size();
	int nb_disc_strike = discretize_strikes.size();

	volatility_surface_discretize = Matrix(nb_disc_maturity, nb_disc_strike, 0);
};



void Volatility_surface::partial_fill_volatility_surface(const int idx1, const int idx2) {
	// Get Matrix rows at Idx1 and Idx2 : 

	double maturity_1 = maturities[idx1];
	double maturity_2 = maturities[idx2];

	std::vector<double> implied_vol_1, implied_vol_2; 
	for (int colIdx = 0; colIdx < strikes.size(); colIdx++) {
		implied_vol_1.push_back(implied_volatility_bs(idx1, colIdx));
		implied_vol_2.push_back(implied_volatility_bs(idx2, colIdx));
	}
	Matrix coeff1 = compute_cubic_spline_interpolation(strikes, implied_vol_1);
	Matrix coeff2 = compute_cubic_spline_interpolation(strikes, implied_vol_2); 

	// We determine all indexes of discretize_maturities corresponding of maturities between T1 and T2 : 
	std::vector<int> maturity_indexes; 
	for (int i = 0; i < discretize_maturities.size(); i++) {
		if (discretize_maturities[i] >= maturity_1 and discretize_maturities[i] <= maturity_2)
			maturity_indexes.push_back(i);
	}
	for (int i = 0; i < maturity_indexes.size(); i++) {
		int maturityIdx = maturity_indexes[i];
		double maturity = discretize_maturities[maturityIdx]; 
		for (int j = 0; j < discretize_strikes.size(); j++) {
			double strike = discretize_strikes[j]; 
			double K_1 = strike * exp((maturity - maturity_1) * RISK_FREE_RATE);
			double K_2 = strike * exp((maturity_2 - maturity) * RISK_FREE_RATE);

			int left_index_1 = left_index(K_1, strikes); 
			int left_index_2 = left_index(K_2, strikes);

			double sigma_1; 
			double sigma_2;

			int strikes_length = strikes.size(); 

			if (left_index_1 == -1) {
				std::vector<double> coeff = coeff1.get_col(0); 
				double D_L = coeff[2]; 
				sigma_1 = implied_volatility_bs(idx1, 0) + D_L * (strike - strikes[0]);
			}
			else if (left_index_1 == (strikes_length - 1)) {
				std::vector<double> coeff = coeff1.get_col(strikes_length - 2);
				double delta_strikes = strikes[strikes_length - 1] - strikes[strikes_length - 2];
				double D_R = 3 * coeff[0] * pow(delta_strikes, 2) + 2 * coeff[1] * delta_strikes + coeff[2] ;
				sigma_1 = implied_volatility_bs(idx1, strikes_length - 1) + D_R * (strike - strikes[strikes_length - 1]);
			}
			else {
				std::vector<double> coeff = coeff1.get_col(left_index_1); 
				double left_strike = strikes[left_index_1]; 
				sigma_1 = cubic_polynomial(left_strike, coeff, strike); 
			}

			if (left_index_2 == -1) {
				std::vector<double> coeff = coeff2.get_col(0);
				double D_L = coeff[2];
				sigma_2 = implied_volatility_bs(idx2, 0) + D_L * (strike - strikes[0]);
			}
			else if (left_index_2 == (strikes_length - 1)) {
				std::vector<double> coeff = coeff2.get_col(strikes_length - 2);
				double delta_strikes = strikes[strikes_length - 1] - strikes[strikes_length - 2];
				double D_R = 3 * coeff[0] * pow(delta_strikes, 2) + 2 * coeff[1] * delta_strikes + coeff[2];
				sigma_2 = implied_volatility_bs(idx2, strikes_length - 1) + D_R * (strike - strikes[strikes_length - 1]);
			}
			else {
				std::vector<double> coeff = coeff2.get_col(left_index_2);
				double left_strike = strikes[left_index_2];
				sigma_2 = cubic_polynomial(left_strike, coeff, strike);
			}
			double delta_square = ((pow(sigma_2, 2) * maturity_2 - pow(sigma_1, 2) * maturity_1) / (maturity_2 - maturity_1));
			volatility_surface_discretize(maturityIdx, j) = sqrt((1 / maturity) * (pow(sigma_1, 2) * maturity_1 + delta_square * (maturity - maturity_1))); 
		}
	}
};


void Volatility_surface::fill_volatility_surface() {
	int nb_maturities = maturities.size(); 
	for (int i = 0; i < nb_maturities - 1; i++) {
		this->partial_fill_volatility_surface(i, i + 1); 
	}
};







