#pragma once

#include "essentiastandardalgorithmwrapper.hpp"

#include <memory>

class Energy : public EssentiaStandardAlgorithmWrapper
{
	std::unique_ptr<Algorithm> energy;
public:
	Energy();
	
	struct Result {
		Real energy;
	};
	
	Result compute(std::vector<Real> const& signal);
};

std::ostream& operator<<(std::ostream& os, Energy::Result const& x);
