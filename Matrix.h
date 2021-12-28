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
	//// Matrix identity
	//Matrix identity(int const size);

	// Assignement operator
	Matrix& operator=(const Matrix& matrix);
	double operator()(const int& row_idx, const int& col_idx) const;
	double& operator()(const int& row_idx, const int& col_idx);
	void operator+=(const double& lambda);
	Matrix operator+(const double& lambda) const;
	void operator-=(const double& lambda);
	Matrix operator-(const double& lambda) const;
	void operator*=(const double& lambda);
	Matrix operator*(const double& lambda) const;
	void operator*=(const Matrix& matrix);
	Matrix operator*(const Matrix& matrix) const;

	// Functions
	Matrix transpose() const;
	void print_matrix() const;
	Matrix dot(const Matrix& matrix) const;

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

	//// Sum between two matrixes
	//Matrix Matrix::sum_matrix(const Matrix& matrix1, const Matrix& matrix2);
	//// Sum between a matrix and a real (right)
	//// 
	//// Difference between a matrix and a real (right)
	//Matrix Matrix::substract_matrix_real(const Matrix& matrix1, const double val);
	//// Product between a matrix and a real (right)
	//Matrix Matrix::dot_matrix_real(const Matrix& matrix1, const double val)
};