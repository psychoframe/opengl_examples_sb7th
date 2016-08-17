#include "sb7.h"
#include "chapter02.h"


GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	// source code for vertex shader
	static const GLchar* vertex_shader_source[] =
	{
		"#version 430 core\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = vec4(0.0, 0.0, 0.5, 1.0);\n"
		"}\n"
	};

	// source code for fragment shader
	static const GLchar* fragment_shader_source[] =
	{
		"#version 430 core\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"	color = vec4(0.0, 0.8, 1.0, 1.0);\n"
		"}\n"
	};

	// create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}