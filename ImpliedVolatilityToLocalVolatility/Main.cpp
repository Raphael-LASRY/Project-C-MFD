#include <iostream>
#include "ImpliedVolatilitySurface.h"

int main()
{
	Vector disctretize_strikes = {};
	for (size_t strike = 1; strike < 21; ++strike)
		disctretize_strikes.push_back(10. * strike);
	Vector disctretize_maturities = {};
	for (size_t maturity = 1; maturity < 121; ++maturity)
		disctretize_maturities.push_back(1.0 / 20 * maturity);

	ImpliedVolatilitySurface IVS("Implied_Volatilities_Example.csv", 0.05, disctretize_strikes, disctretize_maturities);
	IVS.export_implied_volatility("Results.csv");

	return 0;
}