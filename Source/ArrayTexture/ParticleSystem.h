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


class ParticleSystem
{
	struct ParticleSprite
	{
		std::string name;
		std::string spritePath;
		Sprite2D* targetSprite;
	};

public:
	static void InitShaderSystem(std::string shaderPath);
	static void InitSpriteTable(std::string spritePath);

	ParticleSystem();
	ParticleSystem(std::string spriteName);
	~ParticleSystem();

	void Render(ViewManager m_camera, float aspect);
	void Elapse(float deltaTime);
	void SetAttributes(int _amount, float _spawnRadius_Max, float _spawnRadius_Min, float _speed, float _fadeRadius);
	void SetPosition(const vec3 &_position);
	void SetScale(const vec3 &_scale);
	vec3 mPosition = vec3(0, 0, 0);
	vec3 mScale = vec3(1, 1, 1);

private:
	static std::vector<ParticleSprite> ParticleSpriteTable;
	static GLuint program;
	static GLuint vao;
	static GLuint vbo;
	static GLint  mv_matrix;
	static GLint  proj_matrix;

	float time = 0;
	glm::vec2 pos[100];
	int frame[100];
	//	system attributes
	Sprite2D* sprite;
	int amount = 1;				//	MAX = 100
	float spawnRadius_Max = 0;
	float spawnRadius_Min = 0;
	float speed = 0;
	float fadeRadius = 2.0f;	//	start to fade out after this radius
};
std::vector<ParticleSystem::ParticleSprite> ParticleSystem::ParticleSpriteTable
{
	{	"Hit",		"particle_hit.png",		NULL},
	{	"Fire",		"particle_fire.png",	NULL}
};
GLuint ParticleSystem::program;
GLuint ParticleSystem::vao;
GLuint ParticleSystem::vbo;
GLint  ParticleSystem::mv_matrix;
GLint  ParticleSystem::proj_matrix;

void ParticleSystem::InitShaderSystem(std::string shaderPath)
{
	//////////	Init program
	program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = Common::LoadShaderSource((shaderPath + "particle.vs.glsl").c_str());
	char** fsSource = Common::LoadShaderSource((shaderPath + "particle.fs.glsl").c_str());
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

	//Cache uniform variable id
	proj_matrix = glGetUniformLocation(program, "um4p");
	mv_matrix = glGetUniformLocation(program, "um4mv");

	glUseProgram(program);

	//////////	Init buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	pos and frame attributes (layout)
	glm::vec2 pos[100];
	int frame[100];

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(0, 1);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(frame), frame);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_INT, 0, (GLvoid*)(sizeof(pos)));
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void ParticleSystem::InitSpriteTable(std::string spritePath)
{
	for (int i = 0; i < ParticleSpriteTable.size(); i++)
	{
		ParticleSpriteTable[i].targetSprite = new Sprite2D();
		ParticleSpriteTable[i].targetSprite->Init(spritePath + ParticleSpriteTable[i].spritePath, 1, 1, 24, false, 0, 0);
	}
	cout << "SpriteTable initiated.\n";
}

ParticleSystem::ParticleSystem() {}
ParticleSystem::ParticleSystem(std::string spriteName)
{
	for each (ParticleSprite spriteData in ParticleSpriteTable)
	{
		if (spriteData.name == spriteName)
		{
			if (spriteData.targetSprite == NULL)
			{
				cout << "##\t Referencing sprite failed. TargetSprite in the table is NULL.\n";
			}
			sprite = spriteData.targetSprite;
			return;
		}
	}
	//	else...
	sprite = NULL;
	cout << "##\t Cannot find spriteName: \"" << spriteName << "\" in predefined config table.\n";
}
ParticleSystem::~ParticleSystem() {}

void ParticleSystem::SetAttributes(int _amount, float _spawnRadius_Max, float _spawnRadius_Min, float _speed, float _fadeRadius)
{
	amount = _amount <= 100 ? _amount : 100;
	spawnRadius_Max = _spawnRadius_Max;
	spawnRadius_Min = _spawnRadius_Min;
	speed = _speed;
	fadeRadius = _fadeRadius;

	//	init offsets
	for (int i = 0; i < amount; i++)
	{
		pos[i] = RandomPointRadius(spawnRadius_Min, spawnRadius_Max);
	}

	if (amount > 100)
		amount = 100;
}

void ParticleSystem::Elapse(float deltaTime)
{

	///...
}
void ParticleSystem::SetPosition(const vec3 &_position)
{
	mPosition = _position;
}
void ParticleSystem::SetScale(const vec3 &_scale)
{
	mScale = _scale;
}
void ParticleSystem::Render(ViewManager m_camera, float aspect)
{
	if (sprite == NULL)
	{
		cout << "sprite == NULL\n";
		return;
	}

	//	pos and frame attributes (layout)

	/*for (int i = 0; i < amount; i++)
	{
		pos[i] = glm::vec2(i * 0.1f, (i % 10) * 0.1f);
		frame[i] = 0;
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * amount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	///////////////////////////	
	glUseProgram(program);

	glBindVertexArray(vao);

	sprite->Enable();
	glUniformMatrix4fv(mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * scale(mScale) * translate(mPosition)));
	glUniformMatrix4fv(proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, amount);
	sprite->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}