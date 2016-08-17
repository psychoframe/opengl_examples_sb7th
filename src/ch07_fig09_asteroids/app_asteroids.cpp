#include <cstring>
#include <array>
#include <sb7.h>
#include <shader.h>
#include <object.h>
#include <vmath.h>
#include "app_asteroids.h"


AppAsteroids::AppAsteroids()
{
	_renderProgram = 0;
	_mode = MODE_MULTIDRAW;
	_paused = false;
	_vsync = false;
}


void AppAsteroids::init()
{
	static const char title[] = "OpenGL SuperBible - Asteroids";
	sb7::application::init();
	std::memcpy(info.title, title, sizeof(title));
}


void AppAsteroids::startup()
{
	_loadShaders();
	_object.load("../media_objects/asteroids.sbm");

	glGenBuffers(1, &_drawIndirectBuffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _drawIndirectBuffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawArraysIndirectCommand), NULL, GL_STATIC_DRAW);

	DrawArraysIndirectCommand* commands = (DrawArraysIndirectCommand*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER,
		0,
		NUM_DRAWS * sizeof(DrawArraysIndirectCommand),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < NUM_DRAWS; ++i) {
		DrawArraysIndirectCommand& cmd = commands[i];
		_object.get_sub_object_info(i % _object.get_sub_object_count(), cmd.first, cmd.count);
		cmd.primCount = 1;
		cmd.baseInstance = i;
	}

	glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
	glBindVertexArray(_object.get_vao());

	glGenBuffers(1, &_drawIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _drawIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		NUM_DRAWS * sizeof(GLuint),
		NULL,
		GL_STATIC_DRAW);

	GLuint* draw_index = (GLuint*)glMapBufferRange(GL_ARRAY_BUFFER,
		0,
		NUM_DRAWS * sizeof(GLuint),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < NUM_DRAWS; ++i) {
		draw_index[i] = i;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, 0, NULL);
	glVertexAttribDivisor(10, 1);
	glEnableVertexAttribArray(10);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
}


void AppAsteroids::render(double cur_time)
{
	static const float one = 1.0f;
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static double last_time = 0.0;
	static double total_time = 0.0;

	if (!_paused) {
		total_time += (cur_time - last_time);
	}

	last_time = cur_time;
	float t = float(total_time);
	int i = int(total_time * 3.0f);

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	const vmath::mat4 view_matrix = vmath::lookat(vmath::vec3(100.0f * cosf(t * 0.023f), 100.0f * cosf(t * 0.023f), 300.0f * sinf(t * 0.037f) - 600.0f),
		vmath::vec3(0.0f, 0.0f, 260.0f),
		vmath::normalize(vmath::vec3(0.1f - cosf(t * 0.1f) * 0.3f, 1.0f, 0.0f)));

	const vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight,
		1.0f,
		2000.0f);

	glUseProgram(_renderProgram);

	glUniform1f(_uniforms.time, t);
	glUniformMatrix4fv(_uniforms.view_matrix, 1, GL_FALSE, view_matrix);
	glUniformMatrix4fv(_uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(_uniforms.viewproj_matrix, 1, GL_FALSE, proj_matrix * view_matrix);

	glBindVertexArray(_object.get_vao());

	if (_mode == MODE_MULTIDRAW) {
		glMultiDrawArraysIndirect(GL_TRIANGLES, NULL, NUM_DRAWS, 0);
	} else if (_mode == MODE_SEPARATE_DRAWS) {
		for (int j = 0; j < NUM_DRAWS; ++j) {
			GLuint first, count;
			_object.get_sub_object_info(j % _object.get_sub_object_count(), first, count);
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, first, count, 1, j);
		}
	}
}


void AppAsteroids::_loadShaders()
{
	if (_renderProgram) { glDeleteProgram(_renderProgram); }

	std::array<GLuint, 2> shaders;
	shaders[0] = sb7::shader::load("render_vs_ch07_fig09.glsl", GL_VERTEX_SHADER);
	shaders[1] = sb7::shader::load("render_fs_ch07_fig09.glsl", GL_FRAGMENT_SHADER);
	_renderProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

	_uniforms.time = glGetUniformLocation(_renderProgram, "time");
	_uniforms.view_matrix = glGetUniformLocation(_renderProgram, "view_matrix");
	_uniforms.proj_matrix = glGetUniformLocation(_renderProgram, "proj_matrix");
	_uniforms.viewproj_matrix = glGetUniformLocation(_renderProgram, "viewproj_matrix");
}


void AppAsteroids::_onKey(int key, int action)
{
	if (action) {
		switch (key) {
			case 'P':
				_paused = !_paused;
			break;

			case 'V':
				_vsync = !_vsync;
				setVsync(_vsync);
			break;

			case 'D':
				_mode = MODE(_mode + 1);
				if (_mode > MODE_MAX) { _mode = MODE_FIRST; }
			break;

			case 'R':
				_loadShaders();
			break;
		}
	}
}


DECLARE_MAIN(AppAsteroids)