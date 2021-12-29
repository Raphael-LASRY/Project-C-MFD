#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include "math.h"
#include "Matrix.h"

int main()
{
	string filename = "Implied_Volatilities_Example.csv";
	Matrix SIGMA(9, 10);

	ifstream input{ filename };

	if (!input.is_open()) 
	{
		cerr << "Couldn't read file: " << filename << "\n";
		return 1;
	}

	vector<vector<string>> csvRows;

	for (string line; getline(input, line);) 
	{
		istringstream ss(move(line));
		vector<string> row;
		if (!csvRows.empty()) 
		// We expect each row to be as big as the first row
			row.reserve(csvRows.front().size());
		// std::getline can split on other characters, here we use ','
		for (string value; getline(ss, value, ',');)
			row.push_back(move(value));
		csvRows.push_back(std::move(row));
	}

	// Store our matrix
	int row_idx = 0;
	int col_idx = 0;
	for (const vector<string>& row : csvRows) 
	{
		col_idx = 0;
		for (const string& value : row)
		{
			SIGMA(row_idx, col_idx) = stod(value);
			col_idx++;
		}
		row_idx++;
	}

	cout << SIGMA << endl;

	return 0;
}