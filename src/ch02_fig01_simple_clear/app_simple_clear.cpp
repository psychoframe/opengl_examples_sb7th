//
// original : simpleclear
//
#include <cstring>
#include "sb7.h"


class AppSimpleClear : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Simple Clear";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void render(const double cur_time) override
	{
		static const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);
	}
};


DECLARE_MAIN(AppSimpleClear);