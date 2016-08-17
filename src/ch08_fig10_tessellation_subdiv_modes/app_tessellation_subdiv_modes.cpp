//
// original : tesssubdivmodes
//
#include <cstring>
#include <cmath>
#include <array>
#include <fstream>
#include <sb7.h>


void readFileContentText(std::string* resStringPtr, const char* file_path)
{
	std::ifstream ifs(file_path);
	resStringPtr->assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}


class AppTessellationSubdivModes : public sb7::application
{
public:
	AppTessellationSubdivModes()
	{
		_curProgramIndex = 0;
	}


	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Subdivision Modes";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	void startup() override
	{
		std::string vsSource;
		std::string tcsSourceTriangles;
		std::string tesSourceEqual;
		std::string tesSourceFractEven;
		std::string tesSourceFractOdd;
		std::string fsSource;

		readFileContentText(&vsSource, "tesssubdivmodes_vs.glsl");
		readFileContentText(&tcsSourceTriangles, "tesssubdivmodes_tcs_triangle.glsl");
		readFileContentText(&tesSourceEqual, "tesssubdivmodes_tes_equal.glsl");
		readFileContentText(&tesSourceFractEven, "tesssubdivmodes_tes_fract_even.glsl");
		readFileContentText(&tesSourceFractOdd, "tesssubdivmodes_tes_fract_odd.glsl");
		readFileContentText(&fsSource, "tesssubdivmodes_fs.glsl");

		static const char* const vsSources[] = {
			vsSource.c_str(), vsSource.c_str(), vsSource.c_str()
		};

		static const char* const tcsSources[] = {
			tcsSourceTriangles.c_str(), tcsSourceTriangles.c_str(), tcsSourceTriangles.c_str()
		};

		static const char* const tesSources[] = {
			tesSourceEqual.c_str(), tesSourceFractEven.c_str(), tesSourceFractOdd.c_str()
		};

		static const char* const fsSources[] = {
			fsSource.c_str(), fsSource.c_str(), fsSource.c_str()
		};

		for (int i = 0; i < 3; ++i) {
			_renderPrograms[i] = glCreateProgram();
			GLuint vs = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vs, 1, &vsSources[i], NULL);
			glCompileShader(vs);

			GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tcs, 1, &tcsSources[i], NULL);
			glCompileShader(tcs);

			GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tes, 1, &tesSources[i], NULL);
			glCompileShader(tes);

			GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fs, 1, &fsSources[i], NULL);
			glCompileShader(fs);

			glAttachShader(_renderPrograms[i], vs);
			glAttachShader(_renderPrograms[i], tcs);
			glAttachShader(_renderPrograms[i], tes);
			glAttachShader(_renderPrograms[i], fs);
			glLinkProgram(_renderPrograms[i]);

			glDeleteShader(vs);
			glDeleteShader(tcs);
			glDeleteShader(tes);
			glDeleteShader(fs);
		}

		glGenVertexArrays(1, &_vertexArrayObject);
		glBindVertexArray(_vertexArrayObject);

		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


	void render(double cur_time) override
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glUseProgram(_renderPrograms[_curProgramIndex]);
//		glUniform1f(0, sinf((float)cur_time) * 5.0f + 6.0f);
		glUniform1f(0, 5.3f);
		glDrawArrays(GL_PATCHES, 0, 4);
	}


	void shutdown() override
	{
		glDeleteVertexArrays(1, &_vertexArrayObject);

		for (int i = 0; i < _renderPrograms.size(); ++i) {
			glDeleteProgram(_renderPrograms[i]);
		}
	}


	void onKey(int key, int action) override
	{
		if (!action) {
			return;
		}

		switch (key) {
			case 'M':    _curProgramIndex = (_curProgramIndex + 1) % 3;    break;
		}
	}


private:
	std::array<GLuint, 3> _renderPrograms;
	int _curProgramIndex;
	GLuint _vertexArrayObject;
};


DECLARE_MAIN(AppTessellationSubdivModes)