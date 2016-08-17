//
// original : singlepoint
//
#include <cstring>
#include "sb7.h"
#include "chapter02.h"


class AppSinglePoint : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Single Point";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		_renderProgram = compile_shaders();
		glCreateVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &_vertexArrayObject);
		glDeleteProgram(_renderProgram);
	}

	void render(double cur_time) override
	{
		static const GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
//		const GLfloat color[] = { (float)sin(cur_time) * 0.5f + 0.5f, (float)cos(cur_time) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		glUseProgram(_renderProgram);

//		glPointSize(40.0f);    // 그림 2-3. 점을 더 크게 만들기.
		glDrawArrays(GL_POINTS, 0, 1);
	}

private:
	GLuint _renderProgram;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppSinglePoint);