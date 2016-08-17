//
// original : multidrawindirect
//
#ifndef	__CHAPTER07__FIGURE09__ASTEROIDS__H__
#define	__CHAPTER07__FIGURE09__ASTEROIDS__H__


#include <sb7.h>


class AppAsteroids : public sb7::application
{
public:
	AppAsteroids();
	void init() override;
	void startup() override;
	void render(double cur_time) override;

protected:
	struct DrawArraysIndirectCommand
	{
		GLuint count;
		GLuint primCount;
		GLuint first;
		GLuint baseInstance;
	};

	const int NUM_DRAWS = 50000;

	enum MODE
	{
		MODE_FIRST,
		MODE_MULTIDRAW = 0,
		MODE_SEPARATE_DRAWS,
		MODE_MAX = MODE_SEPARATE_DRAWS
	};

	GLuint _renderProgram;
	GLuint _drawIndirectBuffer;
	GLuint _drawIndexBuffer;
	sb7::object _object;

	struct {
		GLint time;
		GLint view_matrix;
		GLint proj_matrix;
		GLint viewproj_matrix;
	} _uniforms;

	MODE _mode;
	bool _paused;
	bool _vsync;

	void _loadShaders();
	void _onKey(int key, int action);
};


#endif