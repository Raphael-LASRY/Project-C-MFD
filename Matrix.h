#pragma once
#include <vector>
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include "math.h"
#include "Matrix.h"
#include <cassert>
#include <stdlib.h> 
#include <iomanip> 

using namespace std;

class Matrix
{
protected:
	int _nrows;
	int _ncols;
	std::vector<std::vector<double>> _data;
public:
	// Default constructor
	Matrix();
	// Constructor with parameters
	Matrix(const int& nrows, const int& ncols, const double& default_val = 0.);
	// Copy constructor
	Matrix(const Matrix& matrix);
	// Destructor
	virtual ~Matrix() = default;

	// Assignement operator
	Matrix& operator=(const Matrix& matrix);

	// Read / set a value at a given index
	double operator()(const int& row_idx, const int& col_idx) const;
	double& operator()(const int& row_idx, const int& col_idx);

	// Basic left operator surcharge
	void operator+=(const double& lambda);
	Matrix operator+(const double& lambda) const;

	void operator+=(const Matrix& matrix);
	Matrix operator+(const Matrix& matrix) const;

	void operator-=(const double& lambda);
	Matrix operator-(const double& lambda) const;

	void operator-=(const Matrix& matrix);
	Matrix operator-(const Matrix& matrix) const;

	void operator*=(const double& lambda);
	Matrix operator*(const double& lambda) const;

	void operator*=(const Matrix& matrix);
	Matrix operator*(const Matrix& matrix) const;

	// Matrix product
	Matrix dot(const Matrix& matrix) const;

	// Functions
	Matrix transpose() const;

	// Getters
	inline int nrows() const
	{
		return _nrows;
	}
	inline int ncols() const
	{
		return _ncols;
	}

	std::vector<double> get_row(const int i);

	std::vector<double> get_col(const int j);

};

// Basic right operator surcharge
Matrix operator+(const double& lambda, const Matrix& matrix);
Matrix operator-(const double& lambda, const Matrix& matrix);
Matrix operator*(const double& lambda, const Matrix& matrix);

// Print operator surcharge
ostream& operator <<(ostream& str, const Matrix& matrix);

Matrix identity(int const size);