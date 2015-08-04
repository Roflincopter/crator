#pragma once

#include <types.h>

#include <vector>
#include <ostream>

using essentia::Real;

struct EnergyChunks {
	std::vector<std::vector<Real>> chunks;
	std::vector<Real> energy;
	std::map<size_t, Real> cache;
	
	EnergyChunks(std::vector<std::vector<Real>> chunks, std::vector<Real> energy);
	
	Real get_energy(int sample_index);
	
	
	
	void build_cache();
};

std::ostream& operator<<(std::ostream& os, EnergyChunks const& ec);
