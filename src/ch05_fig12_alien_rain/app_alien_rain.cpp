//
// original : alienrain
//
#include <cstring>
#include <cmath>
#include <array>
#include "sb7.h"
#include "sb7ktx.h"
#include "shader.h"
#include "vmath.h"


// Random number generator
static unsigned int seed = 0x13371337;


static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^ (seed << 15);
	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}


class AppAlienRain : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Alien Rain";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup() override
	{
		std::array<GLuint, 2> shaders;
		shaders[0] = sb7::shader::load("render_vs_ch05_ex43.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb7::shader::load("render_fs_ch05_ex44.glsl", GL_FRAGMENT_SHADER);
		_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);
		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		_texAlienArray = sb7::ktx::file::load("texture_aliens.ktx");
		glBindTexture(GL_TEXTURE_2D_ARRAY, _texAlienArray);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenBuffers(1, &_rainBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, _rainBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(vmath::vec4), NULL, GL_DYNAMIC_DRAW);

		for (int i = 0; i < 256; ++i) {
			droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
			droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
			droplet_fall_speed[i] = random_float() + 0.2f;
		}

		glBindVertexArray(_vertexArrayObject);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	void render(double cur_time) override
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float t = (float)cur_time;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(_renderProgram);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, _rainBuffer);
		vmath::vec4* droplet = (vmath::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 256 * sizeof(vmath::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i = 0; i < 256; ++i) {
			droplet[i][0] = droplet_x_offset[i];
			droplet[i][1] = 2.0f - fmodf((t + float(i)) * droplet_fall_speed[i], 4.31f);
			droplet[i][2] = t * droplet_rot_speed[i];
			droplet[i][3] = 0.0f;
		}

		glUnmapBuffer(GL_UNIFORM_BUFFER);

		for (int alien_index = 0; alien_index < 256; ++alien_index) {
			glVertexAttribI1i(0, alien_index);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}


protected:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
	GLuint _texAlienArray;
	GLuint _rainBuffer;

	float droplet_x_offset[256];
	float droplet_rot_speed[256];
	float droplet_fall_speed[256];
};


DECLARE_MAIN(AppAlienRain)