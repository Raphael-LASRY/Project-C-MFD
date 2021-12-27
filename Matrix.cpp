#include "Matrix.h"
#include <iostream>
#include <assert.h>

//////////////////////////////////  Matrix //////////////////////////////////////////////

Matrix::Matrix()
	: _nrows(1), _ncols(1)
{
	std::vector<double> init;
	init.push_back(0.);
	_data.push_back(init);
}

Matrix::Matrix(const int& nrows, const int& ncols, const double& default_val)
	:_nrows(nrows), _ncols(ncols)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		std::vector<double> row_vector;
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			row_vector.push_back(default_val);
		_data.push_back(row_vector);
	}
}

Matrix::Matrix(const Matrix& matrix)
	: _nrows(matrix._nrows), _ncols(matrix._ncols)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		std::vector<double> row_vector;
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			row_vector.push_back(matrix._data[row_idx][col_idx]);
		_data.push_back(row_vector);
	}
}

Matrix Matrix::identity(int const size)
{
	Matrix I(size, size);
	for (int row_idx = 0; row_idx < size; row_idx++)
		I(row_idx, row_idx) = 1;
	return I;
}

Matrix& Matrix::operator=(const Matrix& matrix)
{
	if (this != &matrix)
	{
		_nrows = matrix._nrows;
		_ncols = matrix._ncols;

		_data.clear();
		for (int row_idx = 0; row_idx < _nrows; row_idx++)
		{
			std::vector<double> row_vector;
			for (int col_idx = 0; col_idx < _ncols; col_idx++)
				row_vector.push_back(matrix._data[row_idx][col_idx]);
			_data.push_back(row_vector);
		}
	}
	return *this;
}

double& Matrix::operator()(const int& row_idx, const int& col_idx)
{
	return _data[row_idx][col_idx];
}

double Matrix::operator()(const int& row_idx, const int& col_idx) const
{
	return _data[row_idx][col_idx];
}

void Matrix::operator-=(const double& lambda)
{
	(*this) = (*this) + (-1) * lambda;
}

Matrix Matrix::operator-(const double& lambda) const
{
	return (*this) - lambda;
}

void Matrix::operator*=(const double& lambda)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[col_idx][row_idx] *= lambda;
	}
}

Matrix Matrix::operator*(const double& lambda) const
{
	Matrix product(*this);
	product *= lambda;
	return product;
}

void Matrix::operator*=(const Matrix& matrix)
{
	assert(_nrows == matrix._nrows && _ncols == matrix._ncols);
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[col_idx][row_idx] *= matrix(row_idx, col_idx);
	}
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
	Matrix product(*this);
	product *= matrix;
	return product;
}

Matrix Matrix::dot(const Matrix& matrix) const
{
	assert(_ncols == matrix._nrows);
	Matrix product(_nrows, matrix._ncols);
	double sum = 0;
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix._ncols; col_idx++)
		{
			sum = 0;
			for (int col_row_idx = 0; col_row_idx < _ncols; col_row_idx++)
				sum += _data[row_idx][col_row_idx] * matrix(col_row_idx, col_idx);
			product(row_idx, col_idx) = sum;
		}
	}
	return product;
}

Matrix* Matrix::transpose() const
{
	Matrix transpose = Matrix(_ncols, _nrows);

	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			transpose(col_idx, row_idx) = (*this)(row_idx, col_idx);
	}
	return &transpose;
}

void Matrix::print_matrix() const
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		std::cout << "| ";
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			std::cout << (*this)(row_idx, col_idx) << " ";
		std::cout << "|\n";
	}
	std::cout << "\n\n";
}

void Matrix::set_value(const Matrix& matrix, int& i_ligne, int& j_col, double& val)
	:_nrows(matrix._nrows), _ncols(matrix._ncols)
{
	_data((i_ligne-1)*_ncols+j_col, val);
}

Matrix Matrix::sum_matrix(const Matrix& matrix1, const Matrix& matrix2)
{
	assert(matrix1._nrows == matrix2._nrows && matrix1._ncols == matrix2._ncols);
	Matrix sum_matrix = Matrix(matrix1._ncols, matrix1._nrows);
	for (int row_idx = 0; row_idx < matrix1._nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix1._ncols; col_idx++)
			sum_matrix(row_idx, col_idx) = matrix1(row_idx, col_idx) + matrix2(row_idx, col_idx);
	}
	return &sum_matrix;
}

Matrix Matrix::sum_matrix_real(const Matrix& matrix1, const double val)
{
	Matrix sum_mat_real = Matrix(matrix1._ncols, matrix1._nrows);
	for (int row_idx = 0; row_idx < matrix1._nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix1._ncols; col_idx++)
			sum_matrix(row_idx, col_idx) = matrix1(row_idx, col_idx) + val;
	}
	return &sum_mat_real;
}

Matrix Matrix::substract_matrix(const Matrix& matrix1, const Matrix& matrix2)
{
	assert(matrix1._nrows == matrix2._nrows && matrix1._ncols == matrix2._ncols);
	Matrix substract_matrix = Matrix(matrix1._ncols, matrix1._nrows);
	for (int row_idx = 0; row_idx < matrix1._nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix1._ncols; col_idx++)
			substract_matrix(row_idx, col_idx) = matrix1(row_idx, col_idx) - matrix2(row_idx, col_idx);
	}
	return &substract_matrix;
}

Matrix Matrix::substract_matrix_real(const Matrix& matrix1, const double val)
{
	Matrix substract_matrix_real = Matrix(matrix1._ncols, matrix1._nrows);
	for (int row_idx = 0; row_idx < matrix1._nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix1._ncols; col_idx++)
			substract_matrix_real(row_idx, col_idx) = matrix1(row_idx, col_idx) - val;
	}
	return &substract_matrix_real;
}

Matrix Matrix::dot_matrix_real(const Matrix& matrix1, const double val)
{
	Matrix dot_matrix_real = Matrix(matrix1._ncols, matrix1._nrows);
	for (int row_idx = 0; row_idx < matrix1._nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix1._ncols; col_idx++)
			dot_matrix_real(row_idx, col_idx) = matrix1(row_idx, col_idx) * val;
	}
	return &dot_matrix_real;
}