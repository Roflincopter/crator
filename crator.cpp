
#include "monoloader.hpp"
#include "bpmdetection.hpp"
#include "chunker.hpp"
#include "energy.hpp"
#include "monowriter.hpp"
#include "visualizerwindow.hpp"
#include "ziprange.hpp"

#include <boost/format.hpp>

#include <QApplication>

#include <string>
#include <memory>
#include <ostream>

using namespace essentia;

EnergyChunks get_energy_chunks(boost::filesystem::path filename) {
	MonoLoader loader;
	MonoLoader::Result audio = loader.compute(filename);
	
	BPMDetection bpm_detector;
	BPMDetection::Result bpm = bpm_detector.compute(audio.signal);
	
	Chunker chunker;
	Chunker::Result chunks = chunker.compute(audio.signal, bpm.ticks, 4);
	
	Energy energy_calculator;
	std::vector<Real> energies;
	
	auto i = 0;
	for(auto const& chunk : chunks.chunks) {
		auto energy_val = energy_calculator.compute(chunk);
		
		energies.push_back(energy_val.energy);
	}
	
	return EnergyChunks{chunks.chunks, energies};
}

void write_energy_chunks(EnergyChunks energy_chunks) {
	std::vector<int> indices(energy_chunks.chunks.size());
	std::iota(indices.begin(), indices.end(), 0);
	for(auto const& tuple : zip_range(energy_chunks.chunks, energy_chunks.energy, indices)) {
		MonoWriter writer;
		boost::format fmt("%04d");
		fmt % boost::get<2>(tuple);
		std::string chunk_nr_str = fmt.str();
		writer.compute(boost::get<0>(tuple), "chunk_" + chunk_nr_str + "_" + std::to_string(boost::get<1>(tuple)));
	}
}

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
	
	MonoLoader loader;
	MonoLoader::Result audio = loader.compute(filename);
	
	auto energy_chunks = get_energy_chunks(file);
	//write_energy_chunks(energy_chunks);
	//std::cout << std::endl;
	
	//std::cout << energy_chunks << std::endl;
	
	int retval = 0;
	{
		int x = 0;
		
		QSurfaceFormat fmt;
		fmt.setVersion(3, 3);
		fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
		QSurfaceFormat::setDefaultFormat(fmt);
		
		QApplication qapp(x, nullptr);

		auto gui = std::make_shared<VisualizerWindow>();

		gui->show();
		
		gui->visualize(energy_chunks, audio.signal);
		
		retval = qapp.exec();
	}
	
	return retval;
}
