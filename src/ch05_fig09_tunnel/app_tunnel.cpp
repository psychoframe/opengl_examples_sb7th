//
// original : tunnel
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "sb7ktx.h"
#include "shader.h"
#include "vmath.h"


class AppTunnel : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Tunnel";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup() override
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("render_vs_ch05_fig09.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("render_fs_ch05_fig09.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		uniforms.mvp = glGetUniformLocation(_renderProgram, "mvp");
		uniforms.offset = glGetUniformLocation(_renderProgram, "offset");
		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		_texWall = sb7::ktx::file::load("texture_brick.ktx");
		_texCeiling = sb7::ktx::file::load("texture_ceiling.ktx");
		_texFloor = sb7::ktx::file::load("texture_floor.ktx");
		GLuint textures[] = { _texFloor, _texWall, _texCeiling };

		for (int i = 0; i < 3; ++i) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glBindVertexArray(_vertexArrayObject);
	}


	void render(double cur_time) override
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float t = (float)cur_time;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);
		glUseProgram(_renderProgram);

		const float aspect_ratio = (float)info.windowWidth / (float)info.windowHeight;
		vmath::mat4 proj_matrix = vmath::perspective(60.0f, aspect_ratio, 0.1f, 100.0f);

		glUniform1f(uniforms.offset, t * 0.003f);
		GLuint textures[] = { _texWall, _texFloor, _texWall, _texCeiling };

		for (int i = 0; i < 4; ++i) {
			vmath::mat4 mv_matrix = vmath::rotate(90.0f * (float)i, vmath::vec3(0.0f, 0.0f, 1.0f)) *
				vmath::translate(-0.5f, 0.0f, -10.0f) *
				vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(30.0f, 1.0f, 1.0f);
			vmath::mat4 mvp = proj_matrix * mv_matrix;

			glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, mvp);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}


protected:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;

	struct
	{
		GLint mvp;
		GLint offset;
	} uniforms;

	GLuint _texWall;
	GLuint _texCeiling;
	GLuint _texFloor;
};


DECLARE_MAIN(AppTunnel)