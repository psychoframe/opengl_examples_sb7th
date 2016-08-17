//
// original : tessmodes
//
#include <cstring>
#include <array>
#include <string>
#include <fstream>
#include <sb7.h>
#include <sb7color.h>
#include <sb7textoverlay.h>


void readFileContentText(std::string* resStringPtr, const char* file_path)
{
	std::ifstream ifs(file_path);
	resStringPtr->assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
}


class AppTessellationModes : public sb7::application
{
public:
	AppTessellationModes()
	{
		_curProgramIndex = 0;
	}


	void init() override
	{
		static const char title[] = "OpenGL SuperBible - Tessellation Modes";
		sb7::application::init();
		std::memcpy(info.title, title, sizeof(title));
	}


	virtual void startup() override
	{
		std::string vsSource;
		std::string tcsSourceQuads;
		std::string tcsSourceTriangles;
		std::string tcsSourceIsoLines;
		std::string tesSourceQuads;
		std::string tesSourceTriangles;
		std::string tesSourceTrianglesPoints;
		std::string tesSourceIsoLines1;
		std::string tesSourceIsoLines2;
		std::string fsSource;

		readFileContentText(&vsSource, "tessmodes_ch08_fig03_vs.glsl");
		readFileContentText(&tcsSourceQuads, "tessmodes_ch08_ex01_quads_tcs.glsl");
		readFileContentText(&tcsSourceTriangles, "tessmodes_ch08_ex03_triangles_tcs.glsl");
		readFileContentText(&tcsSourceIsoLines, "tessmodes_ch08_ex05_isolines_tcs.glsl");
		readFileContentText(&tesSourceQuads, "tessmodes_ch08_ex02_quads_tes.glsl");
		readFileContentText(&tesSourceTriangles, "tessmodes_ch08_ex04_triangles_tes.glsl");
		readFileContentText(&tesSourceTrianglesPoints, "tessmodes_ch08_fig09_triangles_points_tes.glsl");
		readFileContentText(&tesSourceIsoLines2, "tessmodes_ch08_ex07_isolines_tes.glsl");
		readFileContentText(&tesSourceIsoLines1, "tessmodes_ch08_ex06_isolines_tes.glsl");
		readFileContentText(&fsSource, "tessmodes_ch08_fig03_fs.glsl");

		const char* const vsSources[] = { vsSource.c_str(),
			vsSource.c_str(),
			vsSource.c_str(),
			vsSource.c_str(),
			vsSource.c_str() };

		const char* const tcsSources[] = { tcsSourceQuads.c_str(),
			tcsSourceTriangles.c_str(),
			tcsSourceTriangles.c_str(),
			tcsSourceIsoLines.c_str(),
			tcsSourceIsoLines.c_str() };

		const char* const tesSources[] = { tesSourceQuads.c_str(),
			tesSourceTriangles.c_str(),
			tesSourceTrianglesPoints.c_str(),
			tesSourceIsoLines1.c_str(),
			tesSourceIsoLines2.c_str() };

		const char* const fsSources[] = { fsSource.c_str(),
			fsSource.c_str(),
			fsSource.c_str(),
			fsSource.c_str(),
			fsSource.c_str() };

		_overlay.init(80, 50, "../media_textures/cp437_9x16.ktx");

		for (int i = 0; i < _renderPrograms.size(); ++i) {
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


	virtual void render(double t) override
	{
		glClearBufferfv(GL_COLOR, 0, sb7::color::Black);
		glUseProgram(_renderPrograms[_curProgramIndex]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_PATCHES, 0, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		static const char* mode_names[] = { "QUADS", "TRIANGLES", "QUADS AS POINTS", "ISOLINES-1", "ISOLINES-2" };
		_overlay.clear();
		_overlay.print("Mode: ");
		_overlay.print(mode_names[_curProgramIndex]);
		_overlay.print(" (M toggles)");
		_overlay.draw();
	}

	
	virtual void shutdown() override
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
			case 'M':	_curProgramIndex = (_curProgramIndex + 1) % _renderPrograms.size();	break;
		}
	}


private:
	std::array<GLuint, 5> _renderPrograms;
	int _curProgramIndex;
	GLuint _vertexArrayObject;
	sb7::text_overlay _overlay;
};


DECLARE_MAIN(AppTessellationModes)