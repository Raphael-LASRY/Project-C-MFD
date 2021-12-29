# include "Volatility_surface.h"



Volatility_surface::Volatility_surface(const std::vector<double> maturities_input, std::vector<double> strikes_input, Matrix implied_volatility_bs) {
	maturities = maturities_input;
	strikes = strikes_input;
	implied_volatility_bs = implied_volatility_bs;
}