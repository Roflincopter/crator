#include "energychunks.hpp"

#include "ziprange.hpp"

#include <iostream>
#include <algorithm>

EnergyChunks::EnergyChunks(std::vector<std::vector<essentia::Real> > chunks, std::vector<essentia::Real> energy)
: chunks(chunks)
, energy(energy)
{
	size_t current_index = 0;
	for(auto const& tuple : zip_range(chunks, energy)) {
		cache.emplace(current_index, boost::get<1>(tuple));
		current_index += boost::get<0>(tuple).size();
	}
}

std::ostream& operator<<(std::ostream& os, const EnergyChunks& ec){
	for(auto const& tuple : zip_range(ec.chunks, ec.energy)) {
		os << "samples: [...] size: " << boost::get<0>(tuple).size() << " energy: " << boost::get<1>(tuple) << std::endl;
	}
	return os;
}

essentia::Real EnergyChunks::get_energy(int sample_index)
{
	auto it = cache.lower_bound(sample_index);
	if(it != cache.begin()) {
		--it;
	}
	
	return it->second;
}
