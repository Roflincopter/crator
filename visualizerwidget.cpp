#include "visualizerwidget.hpp"
#include "shaderloader.hpp"

#include <boost/format.hpp>

#include <iostream>

VisualizerWidget::VisualizerWidget(QWidget* parent)
: QOpenGLWidget(parent)
, quad_vbo(0)
, vertexShader(0)
, fragmentShader(0)
, shaderProgram(0)
, posAttrib(0)
, screen_size(0)
, pos_wave_form(0)
, vao(0)
{}

void VisualizerWidget::set_data(EnergyChunks ec, std::vector<essentia::Real> signal)
{
	this->signal = signal;
	
	float sample_per_pixel = signal.size() / float(width + 1);
	
	std::vector<essentia::Real> pos_vals;
	std::vector<essentia::Real> neg_vals;
	
	essentia::Real pos_avg = 0;
	essentia::Real neg_avg = 0;
	int pos_samples = 0;
	int neg_samples = 0;
	int sample_index = 0;
	for(auto&& sample : signal) {
		if(sample_index >= (pos_vals.size() + 1) * sample_per_pixel) {
			pos_vals.push_back(pos_avg / pos_samples);
			pos_avg = 0;
			pos_samples = 0;
			
			neg_vals.push_back(neg_avg / neg_samples);
			neg_avg = 0;
			neg_samples = 0;
		}
		if(sample > 0) {
			pos_avg += sample;
			++pos_samples;
		} else if (sample < 0) {
			neg_avg += sample;
			++neg_samples;
		}
		++sample_index;
	}
	positive_wave_form = pos_vals;
	negative_wave_form = neg_vals;
	
	min_max = {
		*std::min_element(neg_vals.begin(), neg_vals.end()),
		*std::max_element(pos_vals.begin(), pos_vals.end())
	};
}

void VisualizerWidget::initializeGL()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Glew failed to initialize");
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &quad_vbo);
	glDeleteVertexArrays(1, &vao);
	
	GLfloat vertices[] = {
		-1, -1,
		 1, -1,
		 1,  1,
		-1, -1,
		 1,  1,
		-1,  1	
	};
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	width = viewport[2];
	height = viewport[3];
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_source = load_shader("/home/dennis/projects/crator/build/shaders/waveform.vert");
	const char* vertex_shader_source_pointer = vertex_shader_source.data();
	glShaderSource(vertexShader, 1, &vertex_shader_source_pointer, NULL);
	glCompileShader(vertexShader);
	GLint vertex_status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_status);
	if(!vertex_status) {
		std::string buffer(513, '\0');
		glGetShaderInfoLog(vertexShader, 512, NULL, &buffer[0]);
		std::cerr << buffer;
	}
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_source_string = load_shader("/home/dennis/projects/crator/build/shaders/waveform.frag");
	const char* fragment_source_pointer = fragment_source_string.data();
	glShaderSource(fragmentShader, 1, &fragment_source_pointer, NULL);
	glCompileShader(fragmentShader);
	GLint fragment_status;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_status);
	if(!fragment_status) {
		std::cerr << fragment_source_string << std::endl;
		std::string buffer(513, '\0');
		glGetShaderInfoLog(fragmentShader, 512, NULL, &buffer[0]);
		std::cerr << buffer;
	}
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	
	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);
	
	screen_size = glGetUniformLocation(shaderProgram, "screen_size");
	pos_wave_form = glGetUniformLocation(shaderProgram, "pos_wave_form");
	neg_wave_form = glGetUniformLocation(shaderProgram, "neg_wave_form");
	min_max_value = glGetUniformLocation(shaderProgram, "min_max_value");
	
	float magenta[] = {1.0f, 0.0f, 1.0f, 1.0f };
	
	glGenTextures(1, &pos_waveform_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, pos_waveform_tex);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, magenta);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glGenTextures(1, &neg_waveform_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, neg_waveform_tex);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, magenta);
	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_1D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void VisualizerWidget::paintGL()
{	
	glUseProgram(shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, pos_waveform_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, neg_waveform_tex);
	
	using essentia::operator <<;
	std::cout << width << " " << height << std::endl;
	std::cout << positive_wave_form.size() <<" " << positive_wave_form << std::endl;
	std::cout << min_max[0] << " " << min_max[1] << std::endl;
	
	if(positive_wave_form.size() != 0) {
		glUniform2f(screen_size, width, height);
		
		glActiveTexture(GL_TEXTURE0);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, width, 0, GL_RED, GL_FLOAT, positive_wave_form.data());
		glUniform1i(pos_wave_form, 0);
		
		glActiveTexture(GL_TEXTURE1);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, width, 0, GL_RED, GL_FLOAT, negative_wave_form.data());
		glUniform1i(neg_wave_form, 1);
		
		glUniform2f(min_max_value, min_max[0], min_max[1]);
	
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	glBindTexture(GL_TEXTURE_1D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void VisualizerWidget::resizeGL(int w, int h)
{
	width = w;
	height = h;
}

void VisualizerWidget::cleanup()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &quad_vbo);
	glDeleteVertexArrays(1, &vao);
}