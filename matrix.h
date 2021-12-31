#pragma once
#include <vector>
#include <cassert>
#include <iomanip> 
#include <iostream>

using namespace std;

class Matrix
{
protected:
	int _nrows;
	int _ncols;
	vector<vector<double>> _data;
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
		// Element
	inline int nrows() const
	{
		return _nrows;
	}
	inline int ncols() const
	{
		return _ncols;
	}
	// Line / Column
	vector<double> get_row(const int& row_idx) const;

	vector<double> get_col(const int& col_idx) const;
};

// Basic right operator surcharge
Matrix operator+(const double& lambda, const Matrix& matrix);
Matrix operator-(const double& lambda, const Matrix& matrix);
Matrix operator*(const double& lambda, const Matrix& matrix);

// Print operator surcharge
ostream& operator<<(ostream& str, const Matrix& matrix);

// Create the identity matrix
Matrix identity(int const size);
