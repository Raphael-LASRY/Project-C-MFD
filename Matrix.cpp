#include "Matrix.h"
#include <iostream>

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

Matrix* Matrix::transpose() const
{
	Matrix* transpose_ptr = clone();
	// We ensured that transpose_ptr points to an object of derived type

	transpose_ptr->_nrows = _ncols;
	transpose_ptr->_ncols = _nrows;

	for (int row_idx = 0; row_idx < _nrows; row_idx++)
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			(*transpose_ptr)(col_idx, row_idx) = (*this)(row_idx, col_idx);
	return transpose_ptr;
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