#pragma once

#include "ui_visualizerwindow.hpp"

#include "energychunks.hpp"
#include "waveformalgorithm.hpp"

class VisualizerWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit VisualizerWindow(QWidget *parent = 0);
	
	void visualize(std::vector<WaveformData> waveformdata);
	
private:

	Ui::Visualizer ui;
};
