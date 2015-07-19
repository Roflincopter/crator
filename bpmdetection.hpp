#pragma once

#include "essentiastandardalgorithmwrapper.hpp"

#include <memory>

class BPMDetection : public EssentiaStandardAlgorithmWrapper
{
	std::unique_ptr<essentia::standard::Algorithm> bpm;
public:
	BPMDetection();
	
	struct Result {
		Real bpm;
		std::vector<Real> ticks;
		Real confidence;
		std::vector<Real> estimates;
		std::vector<Real> bpm_intervals;
	};
	
	Result compute(std::vector<essentia::Real> const& audio);
};

std::ostream& operator<<(std::ostream& os, BPMDetection::Result const& x);
