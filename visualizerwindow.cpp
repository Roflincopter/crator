#include "visualizerwindow.hpp"

VisualizerWindow::VisualizerWindow(QWidget *parent)
: QMainWindow(parent)
, chunks()
{
	ui.setupUi(this);
}

void VisualizerWindow::visualize(EnergyChunks ec, std::vector<essentia::Real> signal)
{
	ui.oglvisualizer->set_data(ec, signal);
}
