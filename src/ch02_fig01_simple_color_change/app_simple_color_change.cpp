#include <cstring>
#include "sb7.h"


class AppSimpleColorChange : public sb7::application
{
	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Color Change";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}

	void render(const double cur_time) override
	{
		const GLfloat color[] = { (float)sin(cur_time) * 0.5f + 0.5f, (float)cos(cur_time) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
	}
};


DECLARE_MAIN(AppSimpleColorChange);