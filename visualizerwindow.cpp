#include "visualizerwindow.hpp"
#include "visualizerwidget.hpp"

VisualizerWindow::VisualizerWindow(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
}

void VisualizerWindow::visualize(std::vector<WaveformData> waveformdata)
{
	for(auto&& data : waveformdata) {
		auto widget = new VisualizerWidget(data, this);
		ui.verticalLayout->addWidget(widget);
	}
}
