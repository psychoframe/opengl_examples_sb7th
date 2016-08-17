//
// original : fragmentlist
//
#ifndef	__CHAPTER05__FIGURE13__FRAGMENT_LIST__H__
#define	__CHAPTER05__FIGURE13__FRAGMENT_LIST__H__


#include "sb7.h"
#include "object.h"
#include "vmath.h"


class AppFragmentList : public sb7::application
{
public:
	AppFragmentList();

protected:
	GLuint _clearProgram;
	GLuint _appendProgram;
	GLuint _resolveProgram;

	GLuint _uniformsBuffer;
	GLuint _fragmentBuffer;
	GLuint _headPointerImage;
	GLuint _atomicCounterBuffer;
	GLuint _dummyVertexArrayObject;

	struct
	{
		GLuint      color;
		GLuint      normals;
	} _textures;

	struct uniforms_block
	{
		vmath::mat4 mv_matrix;
		vmath::mat4 view_matrix;
		vmath::mat4 proj_matrix;
	};

	struct
	{
		GLint mvp;
	} _uniforms;

	sb7::object _object;

	void init() override;
	void startup() override;
	void render(double cur_time) override;
	void onKey(int key, int action) override;

	void _loadShaders();
};


#endif