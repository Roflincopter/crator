#include "bpmdetection.hpp"

#include <utils/tnt/tnt_array1d_utils.h>

#include <vector>

using namespace essentia;
using namespace standard;

BPMDetection::BPMDetection(boost::filesystem::path filename)
: factory((essentia::init(), AlgorithmFactory::instance()))
, mono_loader(factory.create("MonoLoader", "filename", filename.string()))
, bpm(factory.create("RhythmExtractor2013"))
{
	
}

BPMDetection::BPMResult BPMDetection::calculate()
{
	std::vector<Real> audioBuffer;
	mono_loader->output("audio").set(audioBuffer);
	bpm->input("signal").set(audioBuffer);
	
	BPMResult result;
	bpm->output("bpm").set(result.bpm);
	bpm->output("ticks").set(result.ticks);
	bpm->output("confidence").set(result.confidence);
	bpm->output("estimates").set(result.estimates);
	bpm->output("bpmIntervals").set(result.bpm_intervals);
	
	mono_loader->compute();
	bpm->compute();
	
	return result;
}


std::ostream& operator<<(std::ostream& os, const BPMDetection::BPMResult& x) {
	return os << "bpm: " << x.bpm << std::endl
	          << "ticks: " << x.ticks << std::endl
	          << "confidence: " << x.confidence << std::endl
	          << "estimates: " << x.estimates << std::endl
	          << "bpm_intervals: " << x.bpm_intervals;
}
