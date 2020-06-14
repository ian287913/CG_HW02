#pragma once
#include <string>
#include <vector>
#include "../../Include/Common.h"
#include "../../Include/ViewManager.h"
#include <cmath>
#include <ctime>
#include "TranslateRoteteScaleHelper.h"
#include "Sprite2D.h"
#include "SpriteObject.h"

struct FxTask {
	float waitingTime;
	std::string FxType;	//	"GrayScale", "ColorScale"
	float targetValue;
	float lerpRatio;
};

class WindowShader
{
public:
	static void InitShaderProgram(std::string shaderPath);
	static void BindFrameBuffer();
	static void Render();
	static void ChangeSize(int w, int h);

	//	Task functions
	static void AddFxTask(FxTask task);
	static void UpdateFxTasks(float deltaTime);

	static GLuint program;
	static GLuint vao;
	static GLuint vbo;
	static GLuint FBODataTexture;
	static GLuint FBO;
	static GLuint depthRBO;
	static const GLfloat window_positions[];

	//	uniforms
	static struct Uniforms
	{
		GLint  grayScale;
		GLint  colorOffset;
		GLint  colorScale;
	} uniforms;
	static float grayScale;
	static float colorScale;
	static glm::vec4 colorOffset;

private:
	//	FX queue
	static std::vector<FxTask> FxTaskTable;
	static float targetValue_GrayScale;
	static float lerpRatio_GrayScale;
	static float targetValue_ColorScale;
	static float lerpRatio_ColorScale;

};
const GLfloat WindowShader::window_positions[] =
{
	1.0f,-1.0f,1.0f,0.0f,
	-1.0f,-1.0f,0.0f,0.0f,
	-1.0f,1.0f,0.0f,1.0f,
	1.0f,1.0f,1.0f,1.0f
};

//	uniforms
struct WindowShader::Uniforms WindowShader::uniforms;
float WindowShader::grayScale = 0;
float WindowShader::colorScale = 1;
glm::vec4 WindowShader::colorOffset = glm::vec4(0, 0, 0, 0);
//	program & buffers
GLuint WindowShader::program;
GLuint WindowShader::vao;
GLuint WindowShader::vbo;
GLuint WindowShader::FBODataTexture;
GLuint WindowShader::FBO;
GLuint WindowShader::depthRBO;
//	FX
std::vector<FxTask> WindowShader::FxTaskTable;
float WindowShader::targetValue_GrayScale = 0;
float WindowShader::lerpRatio_GrayScale = 1;
float WindowShader::targetValue_ColorScale = 1;
float WindowShader::lerpRatio_ColorScale = 1;


void WindowShader::InitShaderProgram(std::string shaderPath)
{
	program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = Common::LoadShaderSource((shaderPath + "window.vs.glsl").c_str());
	char** fsSource = Common::LoadShaderSource((shaderPath + "window.fs.glsl").c_str());
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	Common::FreeShaderSource(vsSource);
	Common::FreeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	Common::ShaderLog(vs);
	Common::ShaderLog(fs);

	//Attach Shader to program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	///

	//	attribute
	uniforms.grayScale = glGetUniformLocation(program, "u_grayScale");
	uniforms.colorScale = glGetUniformLocation(program, "u_colorScale");
	///uniforms.colorOffset = glGetUniformLocation(program, "u_color");

	/*fxRadiusID = glGetUniformLocation(windowProgram, "fxRadius");
	colorErrorID = glGetUniformLocation(windowProgram, "colorError");
	zaTimeID = glGetUniformLocation(windowProgram, "time");*/

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_positions), window_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid*)(sizeof(GL_FLOAT) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &FBO);
}
void WindowShader::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void WindowShader::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBODataTexture);
	glBindVertexArray(vao);
	glUseProgram(program);
	glUniform1f(uniforms.grayScale, grayScale);
	glUniform1f(uniforms.colorScale, colorScale);
	///glUniform4f(uniforms.colorOffset, 0, 0, 0, 0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	///glutSwapBuffers();
}

void WindowShader::ChangeSize(int w, int h)
{
	glDeleteRenderbuffers(1, &depthRBO);
	glDeleteTextures(1, &FBODataTexture);
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);

	glGenTextures(1, &FBODataTexture);
	glBindTexture(GL_TEXTURE_2D, FBODataTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBODataTexture, 0);
}

//	FX:
void WindowShader::AddFxTask(FxTask task)
{
	//"GrayScale", "ColorScale"
	if (task.FxType == "GrayScale" || task.FxType == "ColorScale")
	{
		FxTaskTable.push_back(task);
	}
	else
	{
		cout << "##\t Cannot specify task type: " << task.FxType << ".\n";
	}
}
void WindowShader::UpdateFxTasks(float deltaTime)	//	mini second
{
	//	handle queue table
	for (int i = FxTaskTable.size() - 1; i >= 0; i--)
	{
		FxTaskTable[i].waitingTime -= deltaTime;
		if (FxTaskTable[i].waitingTime <= 0)
		{
			//	apply FX
			if (FxTaskTable[i].FxType == "GrayScale")
			{
				targetValue_GrayScale = FxTaskTable[i].targetValue;
				lerpRatio_GrayScale = FxTaskTable[i].lerpRatio;
			}
			else if (FxTaskTable[i].FxType == "ColorScale")
			{
				targetValue_ColorScale = FxTaskTable[i].targetValue;
				lerpRatio_ColorScale = FxTaskTable[i].lerpRatio;
			}

			FxTaskTable.erase(FxTaskTable.begin() + i);
		}
	}

	//	update values
	grayScale = Lerp(grayScale, targetValue_GrayScale, lerpRatio_GrayScale);
	colorScale = Lerp(colorScale, targetValue_ColorScale, lerpRatio_ColorScale);
}