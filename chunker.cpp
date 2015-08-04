#include "chunker.hpp"

#include <algorithm.h>
#include <algorithmfactory.h>

using namespace essentia;

Chunker::Chunker()
: EssentiaStandardAlgorithmWrapper()
, slicer(factory.create("Slicer"))
{}

Chunker::Result Chunker::compute(std::vector<Real> const& audio, const std::vector<Real>& ticks, int group)
{
	slicer->input("audio").set(audio);
	
	std::vector<Real> end_times;
	std::copy_if(ticks.begin(), ticks.end(), std::back_inserter(end_times), [i = 1, group](auto const&) mutable {
		return !(i++ % group);
	});
	
	std::vector<Real> start_times;
	start_times.push_back(0);
	std::copy(end_times.begin(), end_times.end() - 1, std::back_inserter(start_times));
	
	std::string start_key = "startTimes";
	std::string end_key = "endTimes";
	ParameterMap parameters;
	parameters.add(start_key, start_times);
	parameters.add(end_key, end_times);
	
	slicer->configure(parameters);
	
	Result result;
	slicer->output("frame").set(result.chunks);
	slicer->compute();
	return result;
}

std::ostream& operator<<(std::ostream& os, Chunker::Result const& x) {
	return os << x.chunks << std::endl;
}

