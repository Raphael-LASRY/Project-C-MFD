#pragma once
#include "Matrix.h"
#include "additional_functions.h"
#include "Volatility_surface.h"


Volatility_surface::Volatility_surface(const std::vector<double> maturities_input, const std::vector<double> strikes_input,
	const Matrix implied_volatility_input, const int step_maturity_input, const int step_strike_input) {
	maturities = maturities_input;
	strikes = strikes_input;
	implied_volatility_bs = implied_volatility_input;

	step_maturity = step_maturity_input;
	step_strike = step_strike_input;
	
	// Create the volatility surface matrix : 

	int nb_row = maturities.size() + step_strike * (maturities.size() - 1);
	int nb_col = strikes.size() + step_strike * (strikes.size() - 1);
	volatility_surface_discretize = Matrix(nb_row, nb_col, 0); 

	// Create discretize maturities and strikes 
	std::vector<double> disc_maturities, disc_strikes;
	disc_maturities.push_back(maturities[0]);
	disc_strikes.push_back(strikes[0]); 

	for (int maturityIdx = 0; maturityIdx < maturities.size() - 1; maturityIdx++) {
		double delta_maturity = (maturities[maturityIdx + 1] - maturities[maturityIdx]) / (step_maturity+1); 
			for (int j = 0; j < step_maturity; j++) {
				disc_maturities.push_back(maturities[maturityIdx] + (j + 1) * delta_maturity);
		}
			disc_maturities.push_back(maturities[maturityIdx + 1]);
	}

	for (int strikeIdx = 0; strikeIdx < strikes.size() - 1; strikeIdx++) {
		double delta_strike = (strikes[strikeIdx + 1] - strikes[strikeIdx]) / (step_strike+1);
		for (int j = 0; j < step_strike; j++) {
			disc_strikes.push_back(strikes[strikeIdx] + (j + 1) * delta_strike);
		}
		disc_strikes.push_back(strikes[strikeIdx + 1]);
	}
	discretize_maturities = disc_maturities;
	discretize_strikes = disc_strikes;
};


Volatility_surface::Volatility_surface(const std::string filename, const int step_maturity_input, const int step_strike_input) {

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

	step_maturity = step_maturity_input;
	step_strike = step_strike_input;


	// Create the volatility surface matrix : 

	int nb_row = maturities.size() + step_strike * (maturities.size() - 1);
	int nb_col = strikes.size() + step_strike * (strikes.size() - 1);

	volatility_surface_discretize = Matrix(nb_row, nb_col, 0);

	// Create discretize maturities and strikes 
	std::vector<double> disc_maturities, disc_strikes;
	disc_maturities.push_back(maturities[0]);
	disc_strikes.push_back(strikes[0]);

	for (int maturityIdx = 0; maturityIdx < maturities.size() - 1; maturityIdx++) {
		double delta_maturity = (maturities[maturityIdx + 1] - maturities[maturityIdx]) / (step_maturity+1);
		for (int j = 0; j < step_maturity; j++) {
			disc_maturities.push_back(maturities[maturityIdx] + (j + 1) * delta_maturity);
		}
		disc_maturities.push_back(maturities[maturityIdx + 1]);
	}

	for (int strikeIdx = 0; strikeIdx < strikes.size() - 1; strikeIdx++) {
		double delta_strike = (strikes[strikeIdx + 1] - strikes[strikeIdx]) / (step_strike+1);
		for (int j = 0; j < step_strike; j++) {
			disc_strikes.push_back(strikes[strikeIdx] + (j + 1) * delta_strike);
		}
		disc_strikes.push_back(strikes[strikeIdx + 1]);
	}
	discretize_maturities = disc_maturities;
	discretize_strikes = disc_strikes;

};



void Volatility_surface::partial_fill_volatility_surface(const int idx1, const int idx2) {
	// Get Matrix rows at Idx1 and Idx2 : 
	std::vector<double> implied_vol_1, implied_vol_2; 
	for (int colIdx = 0; colIdx < strikes.size(); colIdx++) {
		implied_vol_1.push_back(implied_volatility_bs(idx1, colIdx));
		implied_vol_2.push_back(implied_volatility_bs(idx2, colIdx));
	}
	Matrix coeff1 = compute_cubic_spline_interpolation(strikes, implied_vol_1);
	Matrix coeff2 = compute_cubic_spline_interpolation(strikes, implied_vol_2); 

	int disc_vol_rowIdx;

	// We first fill the rows corresponding to maturities T1 and T2 : 

	disc_vol_rowIdx = idx1 * (step_maturity+1) ;

	for (int strikeIdx = 0; strikeIdx < strikes.size() - 1; strikeIdx++) {

		int disc_strikeIdx = strikeIdx * (step_strike + 1); 

		double lower_strike_vol = implied_vol_1[strikeIdx]; 
		double upper_strike_vol = implied_vol_1[strikeIdx + 1];
		volatility_surface_discretize(disc_vol_rowIdx, strikeIdx * (step_strike+1)) = lower_strike_vol;
		volatility_surface_discretize(disc_vol_rowIdx, (strikeIdx+1) * (step_strike+1)) = upper_strike_vol;

		std::vector<double> coeff;
		for (int i = 0; i < 4;i++) {
			coeff.push_back(coeff1(i, strikeIdx));
		}
		for (int j = 0; j < step_strike;j++) {
			volatility_surface_discretize(disc_vol_rowIdx, disc_strikeIdx + j + 1) = cubic_polynomial(strikes[strikeIdx], coeff, discretize_strikes[disc_strikeIdx + j + 1]);
		}
	}
	
	// Now we fill maturities between T1 and T2
	for (int maturityIdx = 0; maturityIdx < step_maturity; maturityIdx++) {

	}


};

void Volatility_surface::fill_volatility_surface() {
	int nb_maturities = maturities.size(); 
	for (int i = 0; i < nb_maturities - 1; i++) {
		this->partial_fill_volatility_surface(i, i + 1); 
	}
};







