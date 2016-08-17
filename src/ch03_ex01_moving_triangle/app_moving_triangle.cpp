//
// original : movingtri
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"


class AppMovingTriangle : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Single Point";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("vs_ch03_ex01.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch03_ex02.glsl", GL_FRAGMENT_SHADER);
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
		static const GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
//		const GLfloat color[] = { (float)sin(cur_time) * 0.5f + 0.5f, (float)cos(cur_time) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(_renderProgram);

		const GLfloat attrib[] = { (float)sin(cur_time) * 0.5f, (float)cos(cur_time) * 0.6f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);    // update the value of input attribute 0

		glDrawArrays(GL_TRIANGLES, 0, 3);    // draw one triangle
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppMovingTriangle);