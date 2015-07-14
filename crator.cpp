
#include <algorithmfactory.h>
#include <algorithm.h>
#include <scheduler/network.h>
#include <essentia/streaming/algorithms/poolstorage.h>
#include <config.h>

#include <string>
#include <memory>

int main(int argc, char* argv[]) {

	std::string filename;
	if(argc == 2) {
		filename = std::string(argv[1]);
	} else {
		throw std::runtime_error("This program expects 1 argument to an audio file");
	}

	using namespace essentia;
	using namespace essentia::streaming;

	essentia::init();
	
	AlgorithmFactory::init();
	AlgorithmFactory& factory = AlgorithmFactory::instance();
	
	Algorithm* file = factory.create(
		"MonoLoader",
		"filename", filename
	);
	
	Algorithm* fc = factory.create(
		"FrameCutter",
		"frameSize", 1024,
		"hopSize", 512
	);
	Algorithm* spectrum = factory.create(
		"Spectrum"
	);
	Algorithm* frequency_bands = factory.create(
		"FrequencyBands"
	);
	Algorithm* novelty_curve = factory.create(
		"NoveltyCurve"
	);
	Algorithm* bpm_histogram = factory.create(
		"BpmHistogram"
	);
	
	Algorithm* danceability = factory.create(
		"Danceability"
	);
	
	Pool pool;
	
	file->output("audio") >> fc->input("signal");
	fc->output("frame") >> spectrum->input("frame");
	spectrum->output("spectrum") >> frequency_bands->input("spectrum");
	frequency_bands->output("bands") >> novelty_curve->input("frequencyBands");
	novelty_curve->output("novelty") >> bpm_histogram->input("novelty");
	bpm_histogram->output("bpm") >> PC(pool, "bpm");
	bpm_histogram->output("bpmCandidates") >> PC(pool, "bpmCandidates");
	bpm_histogram->output("bpmMagnitudes") >> NOWHERE;
	bpm_histogram->output("tempogram") >> NOWHERE;
	bpm_histogram->output("frameBpms") >> NOWHERE;
	bpm_histogram->output("ticks") >> NOWHERE;
	bpm_histogram->output("ticksMagnitude") >> NOWHERE;
	bpm_histogram->output("sinusoid") >> NOWHERE;
	
	file->output("audio") >> danceability->input("signal");
	danceability->output("danceability") >> PC(pool, "djensbaar");
	
	essentia::scheduler::Network n(file);
	n.run();
	
	std::cout << "djensbaar: " << pool.value<Real>("djensbaar") << std::endl;
	std::cout << "bpm: " << pool.value<Real>("bpm") << std::endl;
	std::cout << "candidates: " << std::endl;
	for(auto&& bpm : pool.value<std::vector<Real>>("bpmCandidates")) {
		std::cout << "\t" << bpm << std::endl;
	}
	
	n.clear();
	essentia::shutdown();
	
	return 0;
}
