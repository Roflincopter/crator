#include "energy.hpp"

Energy::Energy()
: EssentiaStandardAlgorithmWrapper()
, energy(factory.create("Energy"))
{}

Energy::Result Energy::compute(const std::vector<essentia::Real>& signal)
{
	energy->input("array").set(signal);
	
	Result result;
	energy->output("energy").set(result.energy);
	energy->compute();
	return result;
}

std::ostream& operator<<(std::ostream& os, Energy::Result const& x) {
	return os << "Energy: " << x.energy;
}
