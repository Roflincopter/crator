
#include "monoloader.hpp"
#include "bpmdetection.hpp"
#include "chunker.hpp"
#include "energy.hpp"
#include "monowriter.hpp"

#include <boost/format.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range/iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <string>
#include <memory>
#include <ostream>

using namespace essentia;

template<class... Conts>
auto zip_range(Conts&... conts) {
	return boost::make_iterator_range(
		boost::make_zip_iterator(boost::make_tuple(conts.begin()...)),
		boost::make_zip_iterator(boost::make_tuple(conts.end()...))
	);
}

struct EnergyChunks {
	std::vector<std::vector<Real>> chunks;
	std::vector<Real> energy;
	
	friend std::ostream& operator<<(std::ostream& os, EnergyChunks const& ec){
		for(auto const& tuple : zip_range(ec.chunks, ec.energy)) {
			os << "samples: [...] size: " << boost::get<0>(tuple).size() << " energy: " << boost::get<1>(tuple) << std::endl;
		}
		return os;
	}
};

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
	
	auto energy_chunks = get_energy_chunks(file);
	write_energy_chunks(energy_chunks);
	std::cout << std::endl;
	
	std::cout << energy_chunks << std::endl;
	
	std::cout << result << std::endl;
}
