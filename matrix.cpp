#include "matrix.h"

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


void Matrix::operator+=(const double& lambda)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] += lambda;
	}
}

Matrix Matrix::operator+(const double& lambda) const
{
	Matrix sum(*this);
	sum += lambda;
	return sum;
}


Matrix operator+(const double& lambda, const Matrix& matrix)
{
	return matrix + lambda;
}

void Matrix::operator+=(const Matrix& matrix)
{
	assert(_nrows == matrix._nrows && _ncols == matrix._ncols);
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] += matrix(row_idx, col_idx);
	}
}

Matrix Matrix::operator+(const Matrix& matrix) const
{
	Matrix sum(*this);
	sum += matrix;
	return sum;
}

void Matrix::operator-=(const double& lambda)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] -= lambda;
	}
}

Matrix Matrix::operator-(const double& lambda) const
{
	Matrix difference(*this);
	difference -= lambda;
	return difference;
}

Matrix operator-(const double& lambda, const Matrix& matrix)
{
	return (-1) * matrix + lambda;
}

void Matrix::operator-=(const Matrix& matrix)
{
	assert(_nrows == matrix._nrows && _ncols == matrix._ncols);
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] -= matrix(row_idx, col_idx);
	}
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
	Matrix difference(*this);
	difference -= matrix;
	return difference;
}

void Matrix::operator*=(const double& lambda)
{
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] *= lambda;
	}
}

Matrix Matrix::operator*(const double& lambda) const
{
	Matrix product(*this);
	product *= lambda;
	return product;
}

Matrix operator*(const double& lambda, const Matrix& matrix)
{
	return matrix * lambda;
}

void Matrix::operator*=(const Matrix& matrix)
{
	assert(_nrows == matrix._nrows && _ncols == matrix._ncols);
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			_data[row_idx][col_idx] *= matrix(row_idx, col_idx);
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

Matrix Matrix::transpose() const
{
	Matrix transpose = Matrix(_ncols, _nrows);

	for (int row_idx = 0; row_idx < _nrows; row_idx++)
	{
		for (int col_idx = 0; col_idx < _ncols; col_idx++)
			transpose(col_idx, row_idx) = (*this)(row_idx, col_idx);
	}
	return transpose;
}

vector<double> Matrix::get_row(const int& row_idx) const
{
	assert(row_idx < _nrows);
	vector<double> res;
	for (int col_idx = 0; col_idx < _ncols; col_idx++)
		res.push_back((*this)(row_idx, col_idx));
	return res;
}

vector<double> Matrix::get_col(const int& col_idx)  const
{
	assert(col_idx < _ncols);
	vector<double> res;
	for (int row_idx = 0; row_idx < _nrows; row_idx++)
		res.push_back((*this)(row_idx, col_idx));
	return res;
}


ostream& operator<<(ostream& str, const Matrix& matrix) {
	for (int row_idx = 0; row_idx < matrix.nrows(); row_idx++)
	{
		for (int col_idx = 0; col_idx < matrix.ncols(); col_idx++)
			str << setiosflags(ios::fixed) << setprecision(1) << scientific << matrix(row_idx, col_idx) << ' ';
		str << endl;
	}
	return str;
}


Matrix identity(int const size)
{
	Matrix I(size, size);
	for (int row_idx = 0; row_idx < size; row_idx++)
		I(row_idx, row_idx) = 1;
	return I;
}