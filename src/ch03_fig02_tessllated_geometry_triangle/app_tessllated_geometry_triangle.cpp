//
// original : tessellatedgstri
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"


class AppTessllatedGeometryTriangle : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Tessellation and Geometry Shaders";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		std::array<GLuint, 5> shaders;
		shaders[0] = sb7::shader::load("vertex_shader.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("tessellation_control_shader_ch03_ex07.glsl", GL_TESS_CONTROL_SHADER);
		shaders[2] = sb7::shader::load("tessellation_evaluation_shader_ch03_ex08.glsl", GL_TESS_EVALUATION_SHADER);
		shaders[3] = sb7::shader::load("geometry_shader_ch03_ex09.glsl", GL_GEOMETRY_SHADER);
		shaders[4] = sb7::shader::load("fragment_shader.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		glCreateVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);
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
		glPointSize(5.0f);
		glDrawArrays(GL_PATCHES, 0, 3);
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppTessllatedGeometryTriangle);