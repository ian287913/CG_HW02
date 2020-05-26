#pragma once

#include <ShaderObject.h>
#include <vector>

typedef struct CubemapTexture
{
	GLenum type;
	std::string fileName;
};

class CubemapShader: public ShaderObject
{
public:
	CubemapShader();
	~CubemapShader();

	bool Init(const std::vector<CubemapTexture>& textures, const std::vector<CubemapTexture>& textures2);
	bool LoadTextures(const std::vector<CubemapTexture>& textures, GLuint& _textureID);
	void SetMVMat(const glm::mat4& mat);
	void SetPMat(const glm::mat4& mat);
	void Render(int skyboxIndex);
	void UseTexture(bool use, int skyboxIndex);

private:
	GLuint textureID;
	GLuint textureID2;
	GLuint um4mvLocation;
	GLuint um4pLocation;
	GLuint vao, vbo, ebo;

	void CreateBox();
};

