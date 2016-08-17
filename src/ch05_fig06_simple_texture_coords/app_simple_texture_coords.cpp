//
// original : simpletexcoords
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "shader.h"
#include "vmath.h"
#include "object.h"
#include "sb7ktx.h"


class AppSimpleTextureCoords : public sb7::application
{
public:
	AppSimpleTextureCoords()
	{
		_renderProgram = 0;
		_textureIndex = 0;
	}


protected:
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Texture Coordinates";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	virtual void startup() override
	{
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
		static const GLubyte tex_data[] =
		{
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
			B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
			W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
		};
#undef B
#undef W
		glGenTextures(1, &_textureObjects[0]);
		glBindTexture(GL_TEXTURE_2D, _textureObjects[0]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		_textureObjects[1] = sb7::ktx::file::load("texture_pattern_1.ktx");
		_object.load("../media_objects/torus_nrms_tc.sbm");

		_loadShaders();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}


	virtual void shutdown() override
	{
		glDeleteProgram(_renderProgram);
		glDeleteTextures(2, _textureObjects);
	}


	virtual void render(double cur_time) override
	{
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glBindTexture(GL_TEXTURE_2D, _textureObjects[_textureIndex]);
		glUseProgram(_renderProgram);

		vmath::mat4 proj_matrix = vmath::perspective(60.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);
		vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -3.0f) *
			vmath::rotate((float)cur_time * 19.3f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate((float)cur_time * 21.1f, 0.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(_uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
		glUniformMatrix4fv(_uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);

		_object.render();
	}


	void _loadShaders()
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("render_vs_ch05_ex40.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("render_fs_ch05_ex41.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		_uniforms.mv_matrix = glGetUniformLocation(_renderProgram, "mv_matrix");
		_uniforms.proj_matrix = glGetUniformLocation(_renderProgram, "proj_matrix");
	}


	virtual void onKey(int key, int action) override
	{
		if (action) {
			switch (key) {
				case 'R':
					_loadShaders();
				break;

				case 'T':
					++_textureIndex;
					
					if (_textureIndex > 1) {
						_textureIndex = 0;
					}
				break;
			}
		}
	}


protected:
	GLuint _renderProgram;
	GLuint _textureObjects[2];
	GLuint _textureIndex;

	struct
	{
		GLint mv_matrix;
		GLint proj_matrix;
	} _uniforms;

	sb7::object _object;
};


DECLARE_MAIN(AppSimpleTextureCoords);