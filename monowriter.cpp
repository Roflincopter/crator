#include "monowriter.hpp"

using namespace essentia;

MonoWriter::MonoWriter()
: EssentiaStandardAlgorithmWrapper()
, writer(factory.create("MonoWriter"))
{}

void MonoWriter::compute(std::vector<Real> const& signal, boost::filesystem::path filename)
{
	ParameterMap parameters;
	std::string key = "filename";
	parameters.add(key, filename.string());
	
	writer->input("audio").set(signal);
	writer->configure(parameters);
	
	writer->compute();
}

