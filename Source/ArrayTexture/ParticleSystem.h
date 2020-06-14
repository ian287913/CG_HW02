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


struct ParticleUniforms
{
	GLint  mv_matrix;
	GLint  proj_matrix;
	GLint  time;
	GLint  speed;
	GLint  fadeRadius;
	GLint  fadeDistance;
	GLint  fading;
};

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
	static void UpdateInstances(float deltaTime);
	static void RenderInstances(ViewManager m_camera, float aspect);
	/*
		amount *
		name *

		direction	min, max		by radius
		speed:		min, max		if speed < 0, do middle

		spawnRadius = 0, 0
		fadeRadius, fadeDistance = 1.0

		rotation:	min, max = 0, 6.28
		scale:		min, max
		lifetime = 3
		timeSpeed = 1
	*/
	static void CreateInstance(glm::vec3 _position,
		int _amount, std::string _spriteName, glm::vec2 _directionLH, glm::vec2 _speedLH, glm::vec2 _spawnRadiusLH,
		float _fadeRadius, float _fadeDistance, glm::vec2 _rotationLH, glm::vec2 _scaleLH, float _lifetime, float _timeSpeed);
	static void CreateInstance(std::string spriteName, float _lifeTime, int _amount, float _spawnRadius_Max, float _spawnRadius_Min,
		float _speed, float _fadeRadius, float _fadeDistance);

	ParticleSystem();
	ParticleSystem(std::string spriteName);
	~ParticleSystem();

	void Render(ViewManager m_camera, float aspect);
	void Elapse(float deltaTime);
	void SetAttributes(	float _lifeTime, int _amount, float _spawnRadius_Max, float _spawnRadius_Min, 
						float _speed, float _fadeRadius, float _fadeDistance);
	void SetPosition(const vec3 &_position);
	void SetScale(const vec3 &_scale);
	vec3 mPosition = vec3(0, 0, 0);
	vec3 mScale = vec3(1, 1, 1);

private:
	static std::vector<ParticleSystem> particleInstances;
	static std::vector<ParticleSprite> ParticleSpriteTable;
	static GLuint program;
	static GLuint vao;
	static GLuint vbo;
	//	shader parameters - Uniform
	static ParticleUniforms uniforms;
	//	shader parameters - Layout
	struct
	{
		glm::vec2 pos[100];
		glm::vec2 direction[100];
		float scale[100];
		float rotation[100];
		int frame[100];
	} layouts;

	//	time
	float particleTime = 0;

	//	system attributes
	Sprite2D* sprite;
	float lifeTime = 5000;
	int amount = 1;				//	MAX = 100
	float spawnRadius_Max = 0;
	float spawnRadius_Min = 0;
	float speed = 0;
	float fadeRadius = 3.0f;	//	start to fade out after this radius
	float fadeDistance = 2.0f;	//	the distance that alpha: 1 -> 0
};
std::vector<ParticleSystem> ParticleSystem::particleInstances;
std::vector<ParticleSystem::ParticleSprite> ParticleSystem::ParticleSpriteTable
{
	{	"Hit",		"particle_hit.png",		NULL},
	{	"Fire",		"particle_fire.png",	NULL}
};
GLuint ParticleSystem::program;
GLuint ParticleSystem::vao;
GLuint ParticleSystem::vbo;
ParticleUniforms ParticleSystem::uniforms;

void ParticleSystem::InitShaderSystem(std::string shaderPath)
{
	//////////	Clear Instance vector
	particleInstances.clear();

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
	uniforms.proj_matrix = glGetUniformLocation(program, "um4p");
	uniforms.mv_matrix = glGetUniformLocation(program, "um4mv");
	uniforms.time = glGetUniformLocation(program, "time");
	uniforms.speed = glGetUniformLocation(program, "speed");
	uniforms.fadeRadius = glGetUniformLocation(program, "fadeRadius");
	uniforms.fadeDistance = glGetUniformLocation(program, "fadeDistance");

	glUseProgram(program);

	//////////	Init buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	buffer attributes (layout)
	glm::vec2 pos[100];
	int frame[100];
	float scale[100];
	float rotation[100];
	glm::vec2 direction[100];

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame) + sizeof(scale) + sizeof(rotation) + sizeof(direction), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(0, 1);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(frame), frame);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_INT, 0, (GLvoid*)(sizeof(pos)));
	glVertexAttribDivisor(1, 1);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame), sizeof(scale), scale);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(pos) + sizeof(frame)));
	glVertexAttribDivisor(2, 1);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame) + sizeof(scale), sizeof(rotation), rotation);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(pos) + sizeof(frame) + sizeof(scale)));
	glVertexAttribDivisor(3, 1);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame) + sizeof(scale) + sizeof(rotation), sizeof(direction), direction);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(pos) + sizeof(frame) + sizeof(scale) + sizeof(rotation)));
	glVertexAttribDivisor(4, 1);

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
void ParticleSystem::RenderInstances(ViewManager m_camera, float aspect)
{
	for each (ParticleSystem particle in particleInstances)
	{
		particle.Render(m_camera, aspect);
	}
}
void ParticleSystem::UpdateInstances(float deltaTime)
{
	//	update and handle outdated instance
	for (int i = particleInstances.size()-1; i >= 0; i--)
	{
		particleInstances[i].Elapse(deltaTime);
		if (particleInstances[i].lifeTime < 0)
		{
			particleInstances.erase(particleInstances.begin() + i);
		}
	}
}

void ParticleSystem::CreateInstance(glm::vec3 _position,
	int _amount, std::string _spriteName, 
	glm::vec2 _directionLH, glm::vec2 _speedLH, glm::vec2 _spawnRadiusLH,
	float _fadeRadius, float _fadeDistance, 
	glm::vec2 _rotationLH, glm::vec2 _scaleLH, float _lifetime, float _timeSpeed)
{
	ParticleSystem newParticle(_spriteName);

	newParticle.mPosition = _position;
	newParticle.amount = _amount <= 100 ? _amount : 100;

	newParticle.lifeTime = _lifetime * 1000;
	newParticle.spawnRadius_Min = _spawnRadiusLH.x;
	newParticle.spawnRadius_Max = _spawnRadiusLH.y;
	newParticle.speed = _timeSpeed;
	newParticle.fadeRadius = _fadeRadius;
	newParticle.fadeDistance = _fadeDistance;

	for (int i = 0; i < newParticle.amount; i++)
	{
		newParticle.layouts.pos[i] = RandomPointRadius(_spawnRadiusLH.x, _spawnRadiusLH.y);
		newParticle.layouts.scale[i] = RandomFloat(_scaleLH.x, _scaleLH.y);
		newParticle.layouts.rotation[i] = RandomFloat(_rotationLH.x, _rotationLH.y);
		if (_speedLH.x < 0 && _speedLH.y < 0 && LengthOfVec2(newParticle.layouts.pos[i]) != 0)	//	sdirection = origin point ->pos
		{
			newParticle.layouts.direction[i] = NormalizedVec2(newParticle.layouts.pos[i]) * RandomFloat(-_speedLH.x, -_speedLH.y);
		}
		else
		{
			newParticle.layouts.direction[i] = RandomDirection(_directionLH.x, _directionLH.y) * RandomFloat(_speedLH.x, _speedLH.y);
		}
	}

	particleInstances.push_back(newParticle);

}
void ParticleSystem::CreateInstance(std::string spriteName, 
									float _lifeTime, int _amount, float _spawnRadius_Max, float _spawnRadius_Min,
									float _speed, float _fadeRadius, float _fadeDistance)
{
	ParticleSystem newParticle(spriteName);
	newParticle.SetAttributes(_lifeTime, _amount, _spawnRadius_Max, _spawnRadius_Min, _speed, _fadeRadius, _fadeDistance);

	particleInstances.push_back(newParticle);
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

void ParticleSystem::SetAttributes(float _lifeTime, int _amount, float _spawnRadius_Max, float _spawnRadius_Min, float _speed, float _fadeRadius, float _fadeDistance)
{
	lifeTime = _lifeTime;
	amount = _amount <= 100 ? _amount : 100;
	spawnRadius_Max = _spawnRadius_Max;
	spawnRadius_Min = _spawnRadius_Min;
	speed = _speed;
	fadeRadius = _fadeRadius;
	fadeDistance = _fadeDistance;

	//	init offsets
	for (int i = 0; i < amount; i++)
	{
		layouts.pos[i] = RandomPointRadius(spawnRadius_Min, spawnRadius_Max);
		layouts.scale[i] = RandomFloat(0.5f, 0.5f);
		layouts.rotation[i] = RandomFloat(0.0f, 3.14159f * 2.0f);
		layouts.direction[i] = vec2(0.1f, 0.1f);

	}
}

void ParticleSystem::Elapse(float deltaTime)
{
	particleTime += deltaTime;
	lifeTime -= deltaTime;
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

	glBufferSubData(GL_ARRAY_BUFFER, 0, 
					sizeof(layouts.pos),				layouts.pos);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(layouts.pos),
					sizeof(layouts.frame),				layouts.frame);
					///sizeof(GL_INT) * amount,			layouts.frame);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(layouts.pos) + sizeof(layouts.frame),
					sizeof(layouts.scale),				layouts.scale);
					///sizeof(GL_FLOAT) * amount,			layouts.scale);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(layouts.pos) + sizeof(layouts.frame) + sizeof(layouts.scale),
					sizeof(layouts.rotation),			layouts.rotation);
					///sizeof(GL_FLOAT) * amount,			layouts.rotation);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(layouts.pos) + sizeof(layouts.frame) + sizeof(layouts.scale) + sizeof(layouts.rotation),
					sizeof(layouts.direction),			layouts.direction);
					///sizeof(GL_FLOAT) * amount * 2,			layouts.direction);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	///////////////////////////	
	glUseProgram(program);

	glBindVertexArray(vao);

	sprite->Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * scale(mScale) * translate(mPosition)));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glUniform1f(uniforms.time, particleTime * 0.001f);
	glUniform1f(uniforms.speed, speed);
	glUniform1f(uniforms.fadeRadius, fadeRadius);
	glUniform1f(uniforms.fadeDistance, fadeDistance);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, amount);
	sprite->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}