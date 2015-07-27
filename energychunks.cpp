#include "energychunks.hpp"

#include "ziprange.hpp"

std::ostream& operator<<(std::ostream& os, const EnergyChunks& ec){
	for(auto const& tuple : zip_range(ec.chunks, ec.energy)) {
		os << "samples: [...] size: " << boost::get<0>(tuple).size() << " energy: " << boost::get<1>(tuple) << std::endl;
	}
	return os;
}
