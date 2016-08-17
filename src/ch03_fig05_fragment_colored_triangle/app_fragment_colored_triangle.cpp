//
// original : fragcolorfrompos
//
// figure 3.4, 3.5
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"


#define INTERPOLATE_COLOR


class AppFragmentColoredTriangle : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Fragment Colored Triangle";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		std::array<GLuint, 2> shaders;
#ifndef INTERPOLATE_COLOR
		// figure 3.4
		shaders[0] = sb7::shader::load("vs_ch03_ex10.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch03_ex10.glsl", GL_FRAGMENT_SHADER);
#else
		// figure 3.5
		shaders[0] = sb7::shader::load("vs_ch03_ex11.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch03_ex12.glsl", GL_FRAGMENT_SHADER);
#endif
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
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppFragmentColoredTriangle);