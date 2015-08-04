
#include "monoloader.hpp"
#include "bpmdetection.hpp"
#include "chunker.hpp"
#include "energy.hpp"
#include "monowriter.hpp"
#include "visualizerwindow.hpp"
#include "ziprange.hpp"
#include "waveformalgorithm.hpp"

#include <boost/format.hpp>

#include <QApplication>
#include <QSurfaceFormat>

#include <string>
#include <memory>
#include <ostream>

using essentia::Real;

std::vector<Real> get_audio_data(boost::filesystem::path filename) {
	MonoLoader loader;
	MonoLoader::Result audio = loader.compute(filename);
	return audio.signal;
}

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
	
	return EnergyChunks(chunks.chunks, energies);
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

	std::vector<std::string> filenames;
	if(argc >= 2) {
		for(int i = 1; i < argc; ++i) {
			filenames.emplace_back(argv[1]);
		}
	} else {
		throw std::runtime_error("This program expects 1 argument to an audio file");
	}
	
	std::vector<boost::filesystem::path> paths;
	std::copy(filenames.begin(), filenames.end(), std::back_inserter(paths));
	
	for(auto&& path : paths) {
		if(!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path)) {
			throw std::runtime_error(path.string() + "Does not exists or isn't a file");
		}
	}
	
	std::vector<std::vector<Real>> samples;
	std::transform(paths.begin(), paths.end(), std::back_inserter(samples), get_audio_data);
	
	std::vector<EnergyChunks> chunks;
	std::transform(paths.begin(), paths.end(), std::back_inserter(chunks), get_energy_chunks);
	
	int retval = 0;
	{
		int x = 0;
		
		QSurfaceFormat fmt;
		fmt.setVersion(3, 3);
		fmt.setProfile(QSurfaceFormat::CoreProfile);
		QSurfaceFormat::setDefaultFormat(fmt);
		
		QApplication qapp(x, nullptr);

		auto gui = std::make_shared<VisualizerWindow>();

		gui->show();
		
		std::vector<WaveformData> data;
		for(auto&& tuple : zip_range(samples, chunks)) {
			data.emplace_back(boost::get<0>(tuple), boost::get<1>(tuple));
		}
		
		gui->visualize(data);
		
		retval = qapp.exec();
	}
	
	return retval;
}
