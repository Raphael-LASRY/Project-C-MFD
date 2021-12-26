#pragma once
#include <vector>

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
	Matrix& operator=(const Matrix & matrix);
	double operator()(const int& row_idx, const int& col_idx) const;
	double& operator()(const int& row_idx, const int& col_idx);
	Matrix* transpose() const;
	void print_matrix() const;

	// Getters
	inline int nrows() const
	{
		return _nrows;
	}
	inline int ncols() const
	{
		return _ncols;
	}

	// Classic operators

	// Set a value at an index place
	// Sum between two matrixes
	// Sum between a matrix and a real (left and right)
	// Difference between two matrixes
	// Difference between a matrix and a real (left and right)
	// Product between two matrixes
	// Product between a matrix and a real (left and right)
	// Matrix identity

};