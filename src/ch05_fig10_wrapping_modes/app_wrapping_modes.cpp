//
// original : wrapmodes
//
#include <cstring>
#include <array>
#include "sb7.h"
#include "sb7ktx.h"
#include "shader.h"



class AppWrappingModes : public sb7::application
{
public:
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Texture Wrap Modes";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup() override
	{
		// Generate a name for the texture
		glGenTextures(1, &_texture);

		// Load _texture from file
		sb7::ktx::file::load("texture_rightarrows.ktx", _texture);

		// Now bind it to the context using the GL_TEXTURE_2D binding point
		glBindTexture(GL_TEXTURE_2D, _texture);

		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("render_vs_ch05_fig10.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("render_fs_ch05_fig10.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);
	}


	void shutdown(void) override
	{
		glDeleteProgram(_renderProgram);
		glDeleteVertexArrays(1, &_vertexArrayObject);
		glDeleteTextures(1, &_texture);
	}


	void render(double t) override
	{
		static const GLfloat green[] = { 0.0f, 0.1f, 0.0f, 1.0f };
		static const GLfloat yellow[] = { 0.4f, 0.4f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		static const GLenum wrapmodes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT };
		static const float offsets[] = { -0.5f, -0.5f,
			                              0.5f, -0.5f,
			                             -0.5f,  0.5f,
			                              0.5f,  0.5f };

		glUseProgram(_renderProgram);
		glViewport(0, 0, info.windowWidth, info.windowHeight);

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, yellow);

		for (int i = 0; i < 4; ++i) {
			glUniform2fv(0, 1, &offsets[i * 2]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmodes[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmodes[i]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}


private:
	GLuint _texture;
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppWrappingModes);
