#pragma once

#include "energychunks.hpp"

#include <types.h>

#include <GL/glew.h>

#include <QOpenGLWidget>

#include <memory>
#include <array>

class VisualizerWidget : public QOpenGLWidget
{
	Q_OBJECT
	
	GLuint quad_vbo;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLint posAttrib;
	GLint screen_size;
	GLint pos_wave_form;
	GLint neg_wave_form;
	GLint min_max_value;
	GLuint vao;
	GLuint pos_waveform_tex;
	GLuint neg_waveform_tex;
	
	std::vector<essentia::Real> signal;
	GLint width;
	GLint height;
	
	std::vector<essentia::Real> positive_wave_form;
	std::vector<essentia::Real> negative_wave_form;
	std::array<float, 2> min_max;
	
public:
	VisualizerWidget(QWidget* parent);
	void set_data(EnergyChunks ec, std::vector<essentia::Real> signal);
	
	virtual void initializeGL() override final;
	virtual void paintGL() override final;
	virtual void resizeGL(int w, int h) override final;
	
private slots:
	void cleanup();
};

