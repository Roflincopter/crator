#include "essentiastandardalgorithmwrapper.hpp"

EssentiaStandardAlgorithmWrapper::EssentiaStandardAlgorithmWrapper()
: factory((!essentia::isInitialized() ? essentia::init() : void(), AlgorithmFactory::instance()))
{
	if(!essentia::isInitialized()) {
		essentia::init();
	}
}

