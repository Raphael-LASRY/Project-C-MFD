#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include "math.h"
#include "Matrix.h"

int main()
{
	Matrix A(3, 2, 0);
	Matrix B = 2 * identity(3) * 2;
	cout << B << endl;
	return 0;
}