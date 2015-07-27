#pragma once

#include "ui_visualizerwindow.hpp"

#include "energychunks.hpp"

class VisualizerWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit VisualizerWindow(QWidget *parent = 0);
	
	void visualize(EnergyChunks ec, std::vector<essentia::Real> signal);
	
private:
	EnergyChunks chunks;

	Ui::Visualizer ui;
};
