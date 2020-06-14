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

class WindowShader
{
public:
	static void InitShaderProgram(std::string shaderPath);
	static void BindFrameBuffer();
	static void Render();
	static void ChangeSize(int w, int h);

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
	} uniforms;
	static float grayScale;
	static glm::vec4 colorOffset;
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
glm::vec4 WindowShader::colorOffset = glm::vec4(0, 0, 0, 0);

GLuint WindowShader::program;
GLuint WindowShader::vao;
GLuint WindowShader::vbo;
GLuint WindowShader::FBODataTexture;
GLuint WindowShader::FBO;
GLuint WindowShader::depthRBO;

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
	uniforms.grayScale = glGetUniformLocation(program, "grayScale");
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
	cout << "grayScale = " << grayScale << "\n";
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

