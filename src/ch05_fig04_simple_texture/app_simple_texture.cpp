//
// original : simpletexture
//
#include <cstring>
#include <string>
#include <array>
#include "sb7.h"
#include "shader.h"

/*
static void print_shader_log(GLuint shader)
{
	std::string str;
	GLint len;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len != 0)
	{
		str.resize(len);
		glGetShaderInfoLog(shader, len, NULL, &str[0]);
	}

#ifdef _WIN32
	OutputDebugStringA(str.c_str());
#endif
}
*/

class AppSimpleTexture : public sb7::application
{
public:
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Simple Texturing";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup(void) override
	{
		// Generate a name for the _texture
		glGenTextures(1, &_descTexture);

		// Now bind it to the context using the GL_TEXTURE_2D binding point
		glBindTexture(GL_TEXTURE_2D, _descTexture);

		// Specify the amount of storage we want to use for the _texture
		// (2D texture, 8 mipmap levels, 32-bit floating-point RGBA data, 256 x 256 texels)
		glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);

		// Define some data to upload into the _texture
		float* data = new float[256 * 256 * 4];

		// generate__texture() is a function that fills memory with image data
		_generateTexture(data, 256, 256);

		// Assume the _texture is already bound to the GL_TEXTURE_2D target
		glTexSubImage2D(GL_TEXTURE_2D,  // 2D texture
			0,              // Level 0
			0, 0,           // Offset 0, 0
			256, 256,       // 256 x 256 texels, replace entire image
			GL_RGBA,        // Four channel data
			GL_FLOAT,       // Floating point data
			data);          // Pointer to data

		// Free the memory we allocated before - \GL now has our data
		delete[] data;

		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("vs_ch05_fig04.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("fs_ch05_fig04.glsl", GL_FRAGMENT_SHADER);
		_descRenderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

//		glCreateVertexArrays(1, &_descVertexArray);    // <!> 아래와 무슨 차이가 있는지 확인 필요.
		glGenVertexArrays(1, &_descVertexArray);
		glBindVertexArray(_descVertexArray);
	}


	void shutdown(void) override
	{
		glDeleteProgram(_descRenderProgram);
		glDeleteVertexArrays(1, &_descVertexArray);
		glDeleteTextures(1, &_descTexture);
	}


	void render(double cur_time) override
	{
		static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		glUseProgram(_descRenderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}


private:
	void _generateTexture(float * data, int width, int height)
	{
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
				data[(y * width + x) * 4 + 3] = 1.0f;
			}
		}
	}


private:
	GLuint _descTexture;
	GLuint _descRenderProgram;
	GLuint _descVertexArray;
};


DECLARE_MAIN(AppSimpleTexture);
