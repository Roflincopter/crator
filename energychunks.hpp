#pragma once

#include <types.h>

#include <vector>
#include <ostream>

using essentia::Real;

struct EnergyChunks {
	std::vector<std::vector<Real>> chunks;
	std::vector<Real> energy;
};

std::ostream& operator<<(std::ostream& os, EnergyChunks const& ec);
