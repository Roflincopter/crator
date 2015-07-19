#include "monoloader.hpp"

#include <utils/tnt/tnt_array1d_utils.h>

#include <vector>

using namespace essentia;

MonoLoader::MonoLoader()
: EssentiaStandardAlgorithmWrapper()
, loader(factory.create("MonoLoader"))
{}

MonoLoader::Result MonoLoader::compute(boost::filesystem::path file)
{
	essentia::ParameterMap parameters;
	std::string key = "filename";
	parameters.add(key, file.string());

	loader->configure(parameters);
	
	Result result;
	loader->output("audio").set(result.signal);
	
	loader->compute();
	return result;
}

std::ostream& operator<<(std::ostream& os, MonoLoader::Result const& x) {
	return os << "signal: " << x.signal << std::endl;
}
