//
// original : grass
//
#include <cstring>
#include <cmath>
#include <array>
#include "sb7.h"
#include "vmath.h"
#include "sb7ktx.h"
#include "shader.h"


class AppGrass : public sb7::application
{
public:
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Grass";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup(void) override
	{
		static const GLfloat grass_blade[] = {
			-0.3f, 0.0f,
			0.3f, 0.0f,
			-0.20f, 1.0f,
			0.1f, 1.3f,
			-0.05f, 2.3f,
			0.0f, 3.3f
		};

		glGenBuffers(1, &_grassBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _grassBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

		glGenVertexArrays(1, &_grassVertexArrayObject);
		glBindVertexArray(_grassVertexArrayObject);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("vs_ch07_fig07.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch07_fig07.glsl", GL_FRAGMENT_SHADER);
		_grassProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);
		_uniforms.mvpMatrix = glGetUniformLocation(_grassProgram, "mvpMatrix");

		glActiveTexture(GL_TEXTURE1);
		_texGrassLength = sb7::ktx::file::load("texture_grass_length.ktx");

		glActiveTexture(GL_TEXTURE2);
		_texGrassOrientation = sb7::ktx::file::load("texture_grass_orientation.ktx");

		glActiveTexture(GL_TEXTURE3);
		_texGrassColor = sb7::ktx::file::load("texture_grass_color.ktx");

		glActiveTexture(GL_TEXTURE4);
		_texGrassBend = sb7::ktx::file::load("texture_grass_bend.ktx");
	}


	void shutdown(void) override
	{
		glDeleteProgram(_grassProgram);
	}


	void render(double currentTime) override
	{
		float t = (float)currentTime * 0.02f;
		float r = 550.0f;

		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		vmath::mat4 mv_matrix = vmath::lookat(vmath::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
			vmath::vec3(0.0f, -50.0f, 0.0f),
			vmath::vec3(0.0, 1.0, 0.0));
		vmath::mat4 prj_matrix = vmath::perspective(45.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

		glUseProgram(_grassProgram);
		glUniformMatrix4fv(_uniforms.mvpMatrix, 1, GL_FALSE, (prj_matrix * mv_matrix));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glViewport(0, 0, info.windowWidth, info.windowHeight);

		glBindVertexArray(_grassVertexArrayObject);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
	}

private:
	struct
	{
		GLint mvpMatrix;
	} _uniforms;

	GLuint _grassBuffer;
	GLuint _grassVertexArrayObject;
	GLuint _grassProgram;

	GLuint _texGrassColor;
	GLuint _texGrassLength;
	GLuint _texGrassOrientation;
	GLuint _texGrassBend;
};


DECLARE_MAIN(AppGrass);