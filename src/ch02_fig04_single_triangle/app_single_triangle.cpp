//
// original : singletri
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"


class AppSingleTriangle : public sb7::application
{
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Single Triangle";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("vs_ch02_ex08.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch02_ex09.glsl", GL_FRAGMENT_SHADER);
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
		const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glUseProgram(_renderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppSingleTriangle);