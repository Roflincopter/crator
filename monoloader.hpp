#pragma once

#include "essentiastandardalgorithmwrapper.hpp"

#include <algorithm.h>

#include <boost/filesystem.hpp>

#include <memory>
#include <ostream>

using essentia::Real;

class MonoLoader : public EssentiaStandardAlgorithmWrapper
{
	std::unique_ptr<essentia::standard::Algorithm> loader;
public:
	MonoLoader();
	
	struct Result {
		std::vector<Real> signal;
	};
	
	Result compute(boost::filesystem::path file);
};

std::ostream& operator<<(std::ostream& os, MonoLoader::Result const& x);
