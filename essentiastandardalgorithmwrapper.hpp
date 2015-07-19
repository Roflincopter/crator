#pragma once

#include <algorithmfactory.h>
#include <algorithm>

using namespace essentia::standard;
using essentia::standard::Algorithm;
using essentia::Real;

struct EssentiaStandardAlgorithmWrapper
{
protected:
	AlgorithmFactory& factory;
	
public:
	EssentiaStandardAlgorithmWrapper();
};
