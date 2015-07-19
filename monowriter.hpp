#pragma once

#include "essentiastandardalgorithmwrapper.hpp"

#include <boost/filesystem.hpp>

#include <memory>
#include <vector>

class MonoWriter : public EssentiaStandardAlgorithmWrapper
{
	std::unique_ptr<Algorithm> writer;
public:
	MonoWriter();
	
	void compute(std::vector<Real> const& signal, boost::filesystem::path filename);
};

