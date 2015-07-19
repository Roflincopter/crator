#pragma once 

#include "essentiastandardalgorithmwrapper.hpp"

#include <memory>

class Chunker : public EssentiaStandardAlgorithmWrapper 
{
	std::unique_ptr<Algorithm> slicer;
public:
	Chunker();
	
	struct Result {
		std::vector<std::vector<Real>> chunks;
	};
	
	Result compute(std::vector<Real> const& audio, const std::vector<Real>& ticks, int group);
};

std::ostream& operator<<(std::ostream& os, Chunker::Result const& x);
