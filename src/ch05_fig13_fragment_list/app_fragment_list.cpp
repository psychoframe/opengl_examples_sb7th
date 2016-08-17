#include <cstring>
#include <cmath>
#include <array>
#include "sb7.h"
#include "shader.h"
#include "object.h"
#include "vmath.h"
#include "app_fragment_list.h"


AppFragmentList::AppFragmentList()
{
	_clearProgram = 0;
	_appendProgram = 0;
	_resolveProgram = 0;
}


void AppFragmentList::init()
{
	static const char title[] = "OpenGL SuperBible - Fragment List";
	sb7::application::init();
	std::memcpy(info.title, title, sizeof(title));
}


void AppFragmentList::startup()
{
	_loadShaders();
	_object.load("../media_objects/dragon.sbm");

	glGenBuffers(1, &_uniformsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_fragmentBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _fragmentBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * 1024 * 16, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &_atomicCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, NULL, GL_DYNAMIC_COPY);

	glGenTextures(1, &_headPointerImage);
	glBindTexture(GL_TEXTURE_2D, _headPointerImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, 1024, 1024);

	glGenVertexArrays(1, &_dummyVertexArrayObject);
	glBindVertexArray(_dummyVertexArrayObject);
}


void AppFragmentList::onKey(int key, int action)
{
	if (action)
	{
		switch (key)
		{
			case 'R':    _loadShaders();    break;
		}
	}
}


void AppFragmentList::_loadShaders()
{
	if (_clearProgram) { glDeleteProgram(_clearProgram); }
	if (_appendProgram) { glDeleteProgram(_appendProgram); }
	if (_resolveProgram) { glDeleteProgram(_resolveProgram); }

	std::array<GLuint, 2> shaders;
	shaders[0] = sb7::shader::load("clear.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = sb7::shader::load("clear.fs.glsl", GL_FRAGMENT_SHADER);
	_clearProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

	shaders[0] = sb7::shader::load("append.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = sb7::shader::load("append.fs.glsl", GL_FRAGMENT_SHADER);
	_appendProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

	shaders[0] = sb7::shader::load("resolve.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = sb7::shader::load("resolve.fs.glsl", GL_FRAGMENT_SHADER);
	_resolveProgram = sb7::program::link_from_shaders(shaders.data(), shaders.size(), true);

	_uniforms.mvp = glGetUniformLocation(_appendProgram, "mvp");
}


void AppFragmentList::render(double cur_time)
{
	static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLfloat ones[] = { 1.0f };
	const float f = (float)cur_time;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(_clearProgram);
	glBindVertexArray(_dummyVertexArrayObject);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glUseProgram(_appendProgram);

	vmath::mat4 model_matrix = vmath::scale(7.0f);
	vmath::vec3 view_position = vmath::vec3(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f, sinf(f * 0.35f) * 120.0f);
	vmath::mat4 view_matrix = vmath::lookat(view_position, vmath::vec3(0.0f, 30.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));
	vmath::mat4 mv_matrix = view_matrix * model_matrix;
	vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

	glUniformMatrix4fv(_uniforms.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

	static const unsigned int zero = 0;
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, _atomicCounterBuffer);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _fragmentBuffer);
	glBindImageTexture(0, _headPointerImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

	_object.render();

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(_resolveProgram);

	glBindVertexArray(_dummyVertexArrayObject);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


DECLARE_MAIN(AppFragmentList)