
#include "bpmdetection.hpp"

#include <algorithmfactory.h>
#include <algorithm.h>
#include <scheduler/network.h>
#include <essentia/streaming/algorithms/poolstorage.h>

#include <string>
#include <memory>

int main(int argc, char* argv[]) {

	std::string filename;
	if(argc == 2) {
		filename = std::string(argv[1]);
	} else {
		throw std::runtime_error("This program expects 1 argument to an audio file");
	}
	
	boost::filesystem::path file(filename);
	if(!boost::filesystem::exists(file) || !boost::filesystem::is_regular_file(file)) {
		throw std::runtime_error(file.string() + "Does not exists or isn't a file");
	}
	BPMDetection bpm(file);
	
	auto result = bpm.calculate();
	
	std::cout << result << std::endl;
}
