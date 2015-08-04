#pragma once

#include "waveformalgorithm.hpp"
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
	GLint energy;
	GLuint vao;
	GLuint pos_waveform_tex;
	GLuint neg_waveform_tex;
	GLuint energy_tex;
	
	GLint width;
	GLint height;
	
	WaveformData data;
	
public:
	VisualizerWidget(WaveformData data, QWidget* parent = 0);
	
	virtual void initializeGL() override final;
	virtual void paintGL() override final;
	virtual void resizeGL(int w, int h) override final;
	
private slots:
	void cleanup();
};

