﻿//
// original : indexedcube
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"
#include "vmath.h"


// Remove this to draw only a single cube!
#define MANY_CUBES


class AppCubeRotation : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Indexed Cube";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	virtual void startup() override
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("vs_ch05_ex26.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch05_ex27.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		glCreateVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		_moveLocation = glGetUniformLocation(_renderProgram, "mv_matrix");
		_projLocation = glGetUniformLocation(_renderProgram, "proj_matrix");

		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		static const GLushort vertex_indices[] =
		{
			0, 1, 2,
			2, 1, 3,
			2, 3, 4,
			4, 3, 5,
			4, 5, 6,
			6, 5, 7,
			6, 7, 0,
			0, 7, 1,
			6, 0, 2,
			2, 4, 6,
			7, 5, 3,
			7, 3, 1
		};

		static const GLfloat vertex_positions[] =
		{
			-0.25f, -0.25f, -0.25f,
			-0.25f,  0.25f, -0.25f,
			 0.25f, -0.25f, -0.25f,
			 0.25f,  0.25f, -0.25f,
			 0.25f, -0.25f,  0.25f,
			 0.25f,  0.25f,  0.25f,
			-0.25f, -0.25f,  0.25f,
			-0.25f,  0.25f,  0.25f,
		};

		glGenBuffers(1, &_positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

		glEnable(GL_CULL_FACE);
//		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	virtual void render(double cur_time) override
	{
		int i;
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, green);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glUseProgram(_renderProgram);

		vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);
		glUniformMatrix4fv(_projLocation, 1, GL_FALSE, proj_matrix);

#ifdef MANY_CUBES
		for (i = 0; i < 24; i++)
		{
			float f = (float)i + (float)cur_time * 0.3f;
			vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -20.0f) *
									vmath::rotate((float)cur_time * 45.0f, 0.0f, 1.0f, 0.0f) *
									vmath::rotate((float)cur_time * 21.0f, 1.0f, 0.0f, 0.0f) *
									vmath::translate(sinf(2.1f * f) * 2.0f,
													 cosf(1.7f * f) * 2.0f,
													 sinf(1.3f * f) * cosf(1.5f * f) * 2.0f);
			glUniformMatrix4fv(_moveLocation, 1, GL_FALSE, mv_matrix);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		}
#else
		float f = (float)cur_time * 0.3f;
		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -4.0f) *
								vmath::translate(sinf(2.1f * f) * 0.5f,
													cosf(1.7f * f) * 0.5f,
													sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
								vmath::rotate((float)cur_time * 45.0f, 0.0f, 1.0f, 0.0f) *
								vmath::rotate((float)cur_time * 81.0f, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(_moveLocation, 1, GL_FALSE, mv_matrix);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
#endif
	}

	virtual void shutdown() override
	{
		glDeleteVertexArrays(1, &_vertexArrayObject);
		glDeleteProgram(_renderProgram);
		glDeleteBuffers(1, &_positionBuffer);
		glDeleteBuffers(1, &_indexBuffer);    // <?> 버퍼를 삭제하는 구문이 없는 것 같아 추가.
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
	GLuint _positionBuffer;
	GLuint _indexBuffer;
	GLint _moveLocation;
	GLint _projLocation;
};


DECLARE_MAIN(AppCubeRotation)