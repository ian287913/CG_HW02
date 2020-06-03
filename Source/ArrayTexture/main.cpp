#include "../../Include/Common.h"
#include "../../Include/ViewManager.h"
#include <cmath>
#include <ctime>
#include "Sprite2D.h"
#include "SpriteObject.h"
#include <vector>
#include "AnimationConfig.h"

using namespace glm;
using namespace std;

//uniform id
struct
{
	GLint  mv_matrix;
	GLint  proj_matrix;
} uniforms;

struct CharacterSpriteStruct
{
	string fileName;
	Sprite2D* spriteSheet;
	int rowCount, colCount;
	int rowGap, colGap;
};

const std::string ProjectName = "ArrayTexture";
const std::string ShaderPath = "./Shader/" + ProjectName + "/";
const std::string ImagePath = "./Imgs/" + ProjectName + "/";
const int spriteCount = 3;
const int objectCount = 30;

GLuint			program;			//shader program
mat4			proj_matrix;		//projection matrix
mat4			modelMatrix;
float			aspect;

GLuint			textureID;
ViewManager		m_camera;

Sprite2D sprite2D;
Sprite2D sprite2D2;

//	ian
vector< Sprite2D*> CharacterSpriteSheets;
vector< CharacterSpriteStruct> CharacterSpriteStructs;
int CharacterIndex = 0;
int CharacterSpriteFrame = 0;

int rowGap = 0, colGap = 0;

Sprite2D* MarioSpriteSheet;
SpriteObject* MarioObject;
int MarioFrame = 0;
float imageScale = 4.0f;
//

Sprite2D* spriteSheets[spriteCount];
SpriteObject* objects[objectCount];

GLuint vao;
GLuint vbo;
GLuint ebo;

#define DOR(angle) (angle*3.1415/180);
mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}
mat4 scale(float x, float y, float z) {
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}
mat4 rotate(float angle, float x, float y, float z) {
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}

float RandomFloat(float lo, float hi)
{
	return linearRand(lo, hi);
}

glm::vec2 RandomPosition(float xRange, float yRange)
{
	float r = RandomFloat(1.0f, 1000.0f);
	vec2 pos = diskRand(r);
	pos.x *= xRange / r;
	pos.y *= yRange / r;

	return pos;
}

void ReloadSprite()
{
	cout << "Reload sprite: " << CharacterSpriteStructs[CharacterIndex].fileName << "  (rowGap = " << rowGap << ", colGap = " << colGap << ")\n";

	CharacterSpriteStructs[CharacterIndex].rowGap = rowGap;
	CharacterSpriteStructs[CharacterIndex].colGap = colGap;
	///CharacterSpriteStructs[CharacterSpriteStructs.size() - 1].spriteSheet->Init(ImagePath + CharacterSpriteStructs[CharacterIndex].fileName, 12, 6, 24, false, rowGap, colGap);
	CharacterSpriteStructs[CharacterIndex].spriteSheet->Init(ImagePath + CharacterSpriteStructs[CharacterIndex].fileName, CharacterSpriteStructs[CharacterIndex].rowCount, CharacterSpriteStructs[CharacterIndex].colCount, 24, false, rowGap, colGap);

	CharacterSpriteFrame = 0;
}

void LoadCharacterSprite(string spriteName, int _rowCount, int _colCount, int _rowGap, int _colGap)
{
	cout << "LoadCharacterSprite: " << spriteName << "\n";

	colGap = _colGap;
	rowGap = _rowGap;

	cout << "Load sprite: " << spriteName << "  (rowGap = " << _rowGap << ", colGap = " << _colGap << ")\n";

	CharacterSpriteStruct newCharacterStruct;

	newCharacterStruct.fileName = spriteName;
	newCharacterStruct.colGap = _colGap;
	newCharacterStruct.rowGap = _rowGap;
	newCharacterStruct.rowCount = _rowCount;
	newCharacterStruct.colCount = _colCount;
	newCharacterStruct.spriteSheet = new Sprite2D();
	CharacterSpriteStructs.push_back(newCharacterStruct);
	CharacterSpriteStructs[CharacterSpriteStructs.size() - 1].spriteSheet->Init(ImagePath + spriteName, _rowCount, _colCount, 24, false, _rowGap, _colGap);

	CharacterIndex = CharacterSpriteStructs.size() - 1;
	CharacterSpriteFrame = 0;
}

void My_CreateObject()
{
	//	Mario only
	MarioObject = new SpriteObject();
	MarioObject->SetOffsetPerFrame(vec2(RandomFloat(0.3f, 0.5f), 0.f));
	MarioObject->SetPosition(glm::vec2(0, 0));
	MarioObject->AddSprite(MarioSpriteSheet);
	MarioObject->SetStartFrame(0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	pos and frame attributes (layout)
	glm::vec2 pos[1];
	pos[0] = MarioObject->GetPosition();
	int frame[1];
	frame[0] = MarioObject->GetCurrentFrame();

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

void My_LoadTextures()
{
	//	characters
	///LoadCharacterSprite("boss_christmas.png", 12, 6, 0, 0);
	LoadCharacterSprite("/Lionar/f1_support.png", 12, 6, 0, 0);
	LoadCharacterSprite("/Lionar/f1_tank.png", 12, 6, 0, 0);
	//LoadCharacterSprite("neutral_voidhunter.png", 0, 0);
	//LoadCharacterSprite("neutral_trinitywing.png", 0, 0);

	/*for (int i = 0; i < AnimFrameSets.size(); i++)
	{
		cout << "Anim: " << AnimFrameSets[i].setName << " {";
		for (int j = 0; j < AnimFrameSets[i].frames.size(); j++)
		{
			cout << " " << AnimFrameSets[i].frames[j].first << ",";
		}
		cout << " }\n";

	}*/

	//	only Mario
	MarioSpriteSheet = new Sprite2D();
	//MarioSpriteSheet->Init(ImagePath + "Mario.png", 6, 1, 24);
	MarioSpriteSheet->Init(ImagePath + "neutral_voidhunter.png", 10, 10, 24, false, -1, -1);

	return;
}

void My_Init()
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Initialize shaders
	///////////////////////////	
	program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = Common::LoadShaderSource((ShaderPath + "vertex.vs.glsl").c_str());
	char** fsSource = Common::LoadShaderSource((ShaderPath + "fragment.fs.glsl").c_str());
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

	glUseProgram(program);
	///////////////////////////	

	//Load model to shader program
	My_LoadTextures();
	My_CreateObject();
	m_camera.ToggleOrtho();
	m_camera.Zoom(64);
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::vec2 pos[1];
	int frame[1];
	pos[0] = MarioObject->GetPosition();
	frame[0] = CharacterSpriteFrame;//MarioObject->GetCurrentFrame();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * 1, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	///////////////////////////	
	glUseProgram(program);

	glBindVertexArray(vao);

	CharacterSpriteStructs[CharacterIndex].spriteSheet->Enable();
	///MarioSpriteSheet->Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * scale(imageScale, imageScale, 1) * MarioSpriteSheet->GetModelMat()));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	CharacterSpriteStructs[CharacterIndex].spriteSheet->Disable();
	///MarioSpriteSheet->Disable();




	/*for (int spriteID = 0; spriteID < spriteCount; ++spriteID)
	{
		spriteSheets[spriteID]->Enable();

		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * spriteSheets[spriteID]->GetModelMat()));
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
		glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 4, 10, spriteID * 10);
		spriteSheets[spriteID]->Disable();
	}*/

	glBindVertexArray(0);
	glUseProgram(0);
	///////////////////////////	
	//	draw BG


	glutSwapBuffers();
}

//Call to resize the window
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);
}

//Timer event
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}

//Mouse event
void My_Mouse(int button, int state, int x, int y)
{
	m_camera.mouseEvents(button, state, x, y);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		printf("Mouse %d is pressed\n", button);
	}
	printf("%d %d %d %d\n", button, state, x, y);
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
	///m_camera.keyEvents(key);
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);

	switch (key)
	{
	case 'w':
		imageScale += 2.0f;
		break;
	case 's':
		imageScale -= 2.0f;
		break;
	case 'a':
		CharacterSpriteFrame -= 1;
		if (CharacterSpriteFrame < 0) CharacterSpriteFrame = CharacterSpriteStructs[CharacterIndex].spriteSheet->GetCount() - 1;
		cout << "Frame: " << CharacterSpriteFrame << " of " << CharacterSpriteStructs[CharacterIndex].spriteSheet->GetCount() << "\n";
		break;
	case 'd':
		CharacterSpriteFrame = (CharacterSpriteFrame + 1) % CharacterSpriteStructs[CharacterIndex].spriteSheet->GetCount();
		if (CharacterSpriteFrame < 0) CharacterSpriteFrame = CharacterSpriteStructs[CharacterIndex].spriteSheet->GetCount() - 1;
		cout << "Frame: " << CharacterSpriteFrame << " of " << CharacterSpriteStructs[CharacterIndex].spriteSheet->GetCount() << "\n";
		break;
	case 'z':
		CharacterSpriteFrame = 0;
		break;
	case 'q':
		CharacterIndex -= 1;
		if (CharacterIndex < 0) CharacterIndex = CharacterSpriteStructs.size() - 1;
		colGap = CharacterSpriteStructs[CharacterIndex].colGap;
		rowGap = CharacterSpriteStructs[CharacterIndex].rowGap;
		CharacterSpriteFrame = 0;
		break;
	case 'e':
		CharacterIndex = (CharacterIndex + 1) % CharacterSpriteStructs.size();
		colGap = CharacterSpriteStructs[CharacterIndex].colGap;
		rowGap = CharacterSpriteStructs[CharacterIndex].rowGap;
		CharacterSpriteFrame = 0;
		break;
	case 'l':
		ReloadSprite();
		break;
	case '1':
		colGap += 1;
		cout << "colGap: " << colGap << "\n";
		ReloadSprite();
		break;
	case '2':
		colGap -= 1;
		cout << "colGap: " << colGap << "\n";
		ReloadSprite();
		break;
	case '3':
		rowGap += 1;
		cout << "rowGap: " << rowGap << "\n";
		ReloadSprite();
		break;
	case '4':
		rowGap -= 1;
		cout << "rowGap: " << rowGap << "\n";
		ReloadSprite();
		break;
	default:
		break;
	}

}




void My_Mouse_Moving(int x, int y) {
	m_camera.mouseMoveEvent(x, y);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

#ifdef __APPLE__
	//Change working directory to source code path
	chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 720);
	glutCreateWindow(ProjectName.c_str()); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	Common::DumpInfo();
	////////////////////

	//Call custom initialize function
	My_Init();

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutTimerFunc(16, My_Timer, 0);
	glutPassiveMotionFunc(My_Mouse_Moving);
	glutMotionFunc(My_Mouse_Moving);
	////////////////////

	// Enter main event loop.
	glutMainLoop();

	cout << "after loop\n";


	/*while (true)
	{

	}*/

	return 0;
}

