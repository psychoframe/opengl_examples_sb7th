//
// original : instancedattribs
//
#include <cstring>
#include <cmath>
#include <array>
#include "sb7.h"
#include "vmath.h"
#include "shader.h"


class AppInstancedAttributes : public sb7::application
{
public:
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Instanced Attributes";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup(void) override
	{
		static const GLfloat square_vertices[] =
		{
			-1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f,
			1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		static const GLfloat instance_colors[] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f
		};

		static const GLfloat instance_positions[] =
		{
			-2.0f, -2.0f, 0.0f, 0.0f,
			2.0f, -2.0f, 0.0f, 0.0f,
			2.0f,  2.0f, 0.0f, 0.0f,
			-2.0f,  2.0f, 0.0f, 0.0f
		};

		GLuint offset = 0;
		glGenVertexArrays(1, &_squareVertexArrayObject);
		glGenBuffers(1, &_squareBuffer);
		glBindVertexArray(_squareVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, _squareBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices) + sizeof(instance_colors) + sizeof(instance_positions), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(square_vertices), square_vertices);
		offset += sizeof(square_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_colors), instance_colors);
		offset += sizeof(instance_colors);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_positions), instance_positions);
		offset += sizeof(instance_positions);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(square_vertices));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square_vertices) + sizeof(instance_colors)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("render_vs_ch07_fig08.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("render_fs_ch07_fig08.glsl", GL_FRAGMENT_SHADER);
		_squareProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);
	}


	void shutdown(void) override
	{
		glDeleteProgram(_squareProgram);
		glDeleteBuffers(1, &_squareBuffer);
		glDeleteVertexArrays(1, &_squareVertexArrayObject);
	}


	void render(double cur_time) override
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(_squareProgram);
		glBindVertexArray(_squareVertexArrayObject);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);
	}


private:
	GLuint _squareBuffer;
	GLuint _squareVertexArrayObject;
	GLuint _squareProgram;
};


DECLARE_MAIN(AppInstancedAttributes);