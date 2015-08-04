#pragma once

#include "energychunks.hpp"

#include <types.h>

using essentia::Real;

class WaveformData {
	std::vector<Real> signal;
	EnergyChunks chunks;
	
public:
	std::vector<Real> positive_waveform;
	std::vector<Real> negative_waveform;
	std::vector<Real> energy;
	
	WaveformData(std::vector<Real> signal, EnergyChunks chunks);
	
	void calculate_waveform_data(int nr_pixels);
};

std::ostream& operator<<(std::ostream& os, WaveformData const& data);

