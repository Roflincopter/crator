
#include <boost/filesystem.hpp>
#include <essentia/algorithm.h>
#include <essentia/algorithmfactory.h>

using essentia::Real;

class BPMDetection
{
	essentia::standard::AlgorithmFactory& factory;
	std::unique_ptr<essentia::standard::Algorithm> mono_loader;
	std::unique_ptr<essentia::standard::Algorithm> bpm;
public:
	BPMDetection(boost::filesystem::path filename);
	
	struct BPMResult {
		Real bpm;
		std::vector<Real> ticks;
		Real confidence;
		std::vector<Real> estimates;
		std::vector<Real> bpm_intervals;
	};
	
	BPMResult calculate();
};

std::ostream& operator<<(std::ostream& os, BPMDetection::BPMResult const& x);
