#include "../../Include/Common.h"
#include "../../Include/ViewManager.h"
#include <cmath>
#include <ctime>
#include "Sprite2D.h"
#include "SpriteObject.h"
#include <vector>
#include "GameState.h"
#include "ParticleSystem.h"
#include "TranslateRoteteScaleHelper.h"

using namespace glm;
using namespace std;

#define UPDATE_CYCLE 33		//	(mini second)

//uniform id
struct
{
	GLint  mv_matrix;
	GLint  proj_matrix;
} uniforms;

const std::string ProjectName = "Project2_2DGame";
const std::string ShaderPath = "./Shader/ArrayTexture/";
const std::string ImagePath = "./Imgs/ArrayTexture/";

GLuint			program;			//shader program
mat4			proj_matrix;		//projection matrix
mat4			modelMatrix;
float			aspect;

GLuint			textureID;
ViewManager		m_camera;

//	ian
vector<Animation*> animations;
int CharacterIndex = 0;
bool pauseAnimation = false;
float updateSpeed = 1.0f;

Sprite2D* ShadowSprite;
Sprite2D* BackgroundSprite;

float imageScale = 1.0f;
float debug_x = 0.0f;
float debug_y = 0.0f;
//

//  will
GameState* myGameState;
//


GLuint vao;
GLuint vbo;
GLuint ebo;


void SetVaoVbo()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//	pos and frame attributes (layout)
	glm::vec2 pos[1];
	pos[0] = vec2(0,0);
	int frame[1];
	frame[0] = 0;

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
	//	Load all animations
	Animation::InitializeAllSpritesAndSets(ImagePath);

	//	Create all animations
	animations.clear();
	for each (AnimationConfig config in AnimationConfigTable)
	{
		animations.push_back(new Animation(config.name));
	}

	//	shadow
	ShadowSprite = new Sprite2D();
	ShadowSprite->Init(ImagePath + "shadow.png", 1, 1, 24, false, 0, 0);

	//	Background
	BackgroundSprite = new Sprite2D();
	BackgroundSprite->Init(ImagePath + "Background/BG.png", 1, 1, 24, false, 0, 0);

	return;
}

void InitParticle()
{
	ParticleSystem::InitShaderSystem(ShaderPath);
	ParticleSystem::InitSpriteTable(ImagePath);

	/*particleSystem = new ParticleSystem("Fire");
	particleSystem->mPosition = vec3(2.5f, -1, 0);
	particleSystem->SetAttributes(8, 100, 1, 1, 2.0f, 4, 1.0f);*/

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
	SetVaoVbo();

	//	init particle shader
	InitParticle();

	m_camera.ToggleOrtho();
	m_camera.Zoom(64);

	// will
	myGameState = new GameState();
}

//	Draw a single animation (character)
void DrawAnimation(Animation* anim, glm::mat4 _matrix)
{
	glm::vec2 pos[1];
	int frame[1];
	pos[0] = vec2(0,0);
	frame[0] = anim->GetSheetFrame();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * 1, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	///////////////////////////	
	glUseProgram(program);

	glBindVertexArray(vao);

	anim->Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * _matrix * anim->spriteSheet->GetModelMat()));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	anim->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

//	draw only one-frame sprite
void DrawSprite(Sprite2D* _sprite, glm::mat4 _parentMatrix, const vec3& _position, const vec3& _scale)
{
	glm::vec2 pos[1];
	int frame[1];
	pos[0] = glm::vec2(0, 0);
	frame[0] = 0;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * 1, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	///////////////////////////	
	glUseProgram(program);

	glBindVertexArray(vao);

	_sprite->Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * _parentMatrix * translate(_position) * scale(_scale) * _sprite->GetModelMat()));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	_sprite->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////	Draw background		//////////////////////////////////////////////
	
	DrawSprite(BackgroundSprite, translate(0, 0, 0), vec3(0, 0, 0), vec3(9, 9, 1));


	////////////////	Draw shadows		//////////////////////////////////////////////
	mat4 parentMatrix = translate(0, -2, 0) * scale(2, 2, 1);
	DrawSprite(ShadowSprite, parentMatrix, vec3(animations[CharacterIndex]->shadowOffsetX, animations[CharacterIndex]->shadowOffsetY, 0), vec3(animations[CharacterIndex]->shadowScale, animations[CharacterIndex]->shadowScale, 1));
	for (int i = GameObject::actors.size() - 1; i >= 0; i--)
	{
		GameObject* go = GameObject::actors[i];
		// draw
		mat4 trans = translate(go->position, go->distance * GameObject::depthRatio, 0)
			* scale(go->scale * go->facing, go->scale, 1);
		DrawSprite(
			ShadowSprite, 
			trans, 
			vec3(go->sprite->shadowOffsetX, 
				go->sprite->shadowOffsetY, 0),
			vec3(go->sprite->shadowScale,
				go->sprite->shadowScale, 1));
	}

	////////////////	Draw characters		//////////////////////////////////////////////

	DrawAnimation(animations[CharacterIndex], parentMatrix);
	vector<GameObject*> sortedGO = GameObject::actors;
	for (int i = sortedGO.size() - 1; i >= 0; i--)
	{
		GameObject* go = sortedGO[i];
		for (int j = i - 1; j >= 0; j--)
		{
			GameObject* goCompare = sortedGO[j];
			if (goCompare->distance > go->distance)
			{
				// switch
				sortedGO[i] = goCompare;
				sortedGO[j] = go;
				go = goCompare;
			}
		}
		// draw
		mat4 trans = translate(go->position, go->height + go->distance * GameObject::depthRatio, 0) 
			* scale(go->scale * go->facing, go->scale, 1);
		DrawAnimation(go->sprite, trans);
	}

	////////////////	Draw foreground		//////////////////////////////////////////////

	ParticleSystem::RenderInstances(m_camera, aspect);

	////////////////	Draw UI				//////////////////////////////////////////////



	glutSwapBuffers();
}


//Call to resize the window
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);
}

//Timer event (Update)
void My_Timer(int val)
{
	if (!pauseAnimation)
	{
		animations[CharacterIndex]->Elapse(((float)UPDATE_CYCLE) * updateSpeed);
		//	ParticleSystem
		ParticleSystem::UpdateInstances(((float)UPDATE_CYCLE) * updateSpeed);
	}

	if (myGameState != NULL)
	{
		myGameState->Update(((float)UPDATE_CYCLE) * updateSpeed);
	}

	glutPostRedisplay();
	glutTimerFunc(UPDATE_CYCLE, My_Timer, val);
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
		ParticleSystem::CreateInstance("Fire", 8000, 5, 0.05f, 0, 16.0f, 0.5, 0.3f);
		debug_y += 0.2f;
		cout << "debug_y = " << debug_y << "\n";
		break;
	case 's':
		debug_y -= 0.2f;
		cout << "debug_y = " << debug_y << "\n";
		break;
	case 'd':
		debug_x += 0.2f;
		cout << "debug_x = " << debug_x << "\n";
		break;
	case 'a':
		debug_x -= 0.2f;
		cout << "debug_x = " << debug_x << "\n";
		break;
	/*case 'a':
		updateSpeed -= 0.2f;
		cout << "updateSpeed = " << updateSpeed << "\n";
		break;
	case 'd':
		updateSpeed += 0.2f;
		cout << "updateSpeed = " << updateSpeed << "\n";
		break;*/
	case 'z':
		pauseAnimation = !pauseAnimation;
		cout << "pauseAnimation = " << pauseAnimation << "\n";
		break;
	case 'q':
		CharacterIndex -= 1;
		if (CharacterIndex < 0) CharacterIndex = animations.size() - 1;
		break;
	case 'e':
		CharacterIndex = (CharacterIndex + 1) % animations.size();
		break;
	case '1':
		animations[CharacterIndex]->SetCurrentSet("idle");
		break;
	case '2':
		animations[CharacterIndex]->SetCurrentSet("run");
		break;
	case '3':
		animations[CharacterIndex]->SetCurrentSet("attack");
		break;
	case '4':
		animations[CharacterIndex]->SetCurrentSet("hit");
		break;
	case '5':
		animations[CharacterIndex]->SetCurrentSet("die");
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
	glutInitWindowSize(1800, 720);
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

