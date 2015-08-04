#include "waveformalgorithm.hpp"

#include <algorithm>

std::ostream&operator<<(std::ostream& os, const WaveformData& data)
{
	using essentia::operator<<;
	return os << "pos_vals: " << data.positive_waveform << std::endl
	          << "neg_vals: " << data.negative_waveform << std::endl
	          << "energy:" << data.energy;
}


WaveformData::WaveformData(std::vector<essentia::Real> signal, EnergyChunks chunks)
: signal(signal)
, chunks(chunks)
{}

void WaveformData::calculate_waveform_data(int nr_pixels)
{
	float sample_per_pixel = signal.size() / float(nr_pixels + 1);
	
	std::vector<essentia::Real> pos_vals;
	pos_vals.reserve(nr_pixels);
	std::vector<essentia::Real> neg_vals;
	neg_vals.reserve(nr_pixels);
	std::vector<essentia::Real> energy;
	energy.reserve(nr_pixels);
	
	essentia::Real pos_avg = 0;
	essentia::Real neg_avg = 0;
	essentia::Real energy_avg = 0;
	int pos_samples = 0;
	int neg_samples = 0;
	int energy_samples = 0;
	int sample_index = 0;
	for(auto&& sample : signal) {
		if(sample_index >= (pos_vals.size() + 1) * sample_per_pixel) {
			pos_vals.push_back(pos_avg / pos_samples);
			pos_avg = 0;
			pos_samples = 0;
			
			neg_vals.push_back(neg_avg / neg_samples);
			neg_avg = 0;
			neg_samples = 0;
			
			energy.push_back(energy_avg / energy_samples);
			energy_avg = 0;
			energy_samples = 0;
		}
		if(sample > 0) {
			pos_avg += sample;
			++pos_samples;
		} else if (sample < 0) {
			neg_avg += sample;
			++neg_samples;
		}
		energy_avg += chunks.get_energy(sample_index);
		++energy_samples;
		
		++sample_index;
	}
	
	std::pair<Real, Real> min_max = {
		*std::min_element(neg_vals.begin(), neg_vals.end()),
		*std::max_element(pos_vals.begin(), pos_vals.end())
	};
	
	for(auto&& sample : neg_vals) {
		sample /= min_max.first;
	}
	
	for(auto&& sample : pos_vals) {
		sample /= min_max.second;
	}
	
	std::pair<Real, Real> energy_min_max = {
		*std::min_element(energy.begin(), energy.end()),
		*std::max_element(energy.begin(), energy.end())
	};
	
	Real difference = energy_min_max.second - energy_min_max.first;
	
	for(auto& e : energy) {
		e -= energy_min_max.first;
		e /= difference;
	}
	
	positive_waveform = pos_vals;
	negative_waveform = neg_vals;
	this->energy = energy;
}
