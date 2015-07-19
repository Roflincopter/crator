#include "bpmdetection.hpp"

#include <vector>

using namespace essentia;

BPMDetection::BPMDetection()
: EssentiaStandardAlgorithmWrapper()
, bpm(factory.create("RhythmExtractor2013"))
{
	
}

BPMDetection::Result BPMDetection::compute(std::vector<Real> const& audio)
{
	bpm->input("signal").set(audio);
	
	Result result;
	bpm->output("bpm").set(result.bpm);
	bpm->output("ticks").set(result.ticks);
	bpm->output("confidence").set(result.confidence);
	bpm->output("estimates").set(result.estimates);
	bpm->output("bpmIntervals").set(result.bpm_intervals);
	
	bpm->compute();
	return result;
}


std::ostream& operator<<(std::ostream& os, BPMDetection::Result const& x) {
	return os << "bpm: " << x.bpm << std::endl
	          << "ticks: " << x.ticks << std::endl
	          << "confidence: " << x.confidence << std::endl
	          << "estimates: " << x.estimates << std::endl
	          << "bpm_intervals: " << x.bpm_intervals;
}
