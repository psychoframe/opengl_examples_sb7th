//
// original : tessellatedtri
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"


class AppTessllatedTriangle : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Tessellated Triangle";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		std::array<GLuint, 4> shaders;
		shaders[0] = sb7::shader::load("vertex_shader.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("tessellation_control_shader_ch03_ex07.glsl", GL_TESS_CONTROL_SHADER);
		shaders[2] = sb7::shader::load("tessellation_evaluation_shader_ch03_ex08.glsl", GL_TESS_EVALUATION_SHADER);
		shaders[3] = sb7::shader::load("fragment_shader.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		glCreateVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &_vertexArrayObject);
		glDeleteProgram(_renderProgram);
	}

	void render(double cur_time) override
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(_renderProgram);
		glDrawArrays(GL_PATCHES, 0, 3);
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppTessllatedTriangle);