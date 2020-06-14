#include "../../Include/Common.h"
#include "../../Include/ViewManager.h"
#include <cmath>
#include <ctime>
#include "Sprite2D.h"
#include "SpriteObject.h"
#include <vector>
#include "GameState.h"
#include "TranslateRoteteScaleHelper.h"
#include "WindowShader.h"

using namespace glm;
using namespace std;

#define UPDATE_CYCLE 33		//	(mini second)
#define UINUM 11				//	UI number

//uniform id
struct
{
	GLint  mv_matrix;
	GLint  proj_matrix;
	GLint  fading;
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
Sprite2D* DebugSprite;

float imageScale = 1.0f;
float debug_x = 0.0f;
float debug_y = 0.0f;
bool DEBUG_MODE = false;
//

//  will
GameState* myGameState;
bool isGameOver = false;
bool isWin = false;
float camara_shape[2] = { 0, 0 };
void UIButton(float x, float y);
Sprite2D* UI_Button_chara_back;
Sprite2D* UI_Button_chara_frame;
Sprite2D* UI_Button_money;
Sprite2D* UI_Button_laser;
Sprite2D* UI_black;
Sprite2D* UI_victory_title;
Sprite2D* UI_defeat_title;
Sprite2D* UI_Button_restart;
Animation* UI_Button_chara[CHARNUM];
const string character_names[CHARNUM] = {"L_Tank", "L_Ranger", "L_Tank", "L_Tank", "L_Tank"};
const float character_scale[CHARNUM] = {1.8f, 2.16f, 1.8f, 1.8f, 1.8f};
const float UI_Button_chara_size = 0.3f;
const float UI_Button_money_size = 0.9f;
const float UI_Button_laser_size = 0.3f;
const float UI_Button_restart_size[] = { 2.0f, 0.6f };
const float UI_trans[UINUM][3] = 
{ 
	{-3.6f,		-3.6f,		1.8f},
	{-1.8f,		-3.6f,		1.8f},
	{0,			-3.6f,		1.8f},
	{1.8f,		-3.6f,		1.8f},
	{3.6f,		-3.6f,		1.8f},

	{-9.0f,		-3.8f,		3.0f},
	{9.0f,		-3.6f,		3.0f},

	{	0,			0,		10.0f},	// black
	{	0,			0,		3.0f},	// victory title
	{	0,			0,		3.0f},	// defeat title
	{0.0f,		-3.0f,		1.0f}	// restart button
};
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

	// UI
	UI_Button_chara_back = new Sprite2D();
	UI_Button_chara_back->Init(ImagePath + "characterButtonBack.png", 1, 1, 24, false, 0, 0);
	UI_Button_chara_frame = new Sprite2D();
	UI_Button_chara_frame->Init(ImagePath + "card_background.png", 1, 1, 24, false, 0, 0);
	for (int i = 0; i < CHARNUM; i++)
	{
		Animation* anim = new Animation(character_names[i]);
		UI_Button_chara[i] = anim;
	}
	UI_Button_money = new Sprite2D();
	UI_Button_money->Init(ImagePath + "Money_button.png", 1, 1, 24, false, 0, 0);
	UI_black = new Sprite2D();
	UI_black->Init(ImagePath + "black.png", 1, 1, 24, false, 0, 0);
	UI_victory_title = new Sprite2D();
	UI_victory_title->Init(ImagePath + "victory_title.png", 1, 1, 24, false, 0, 0);
	UI_defeat_title = new Sprite2D();
	UI_defeat_title->Init(ImagePath + "defeat_title.png", 1, 1, 24, false, 0, 0);
	UI_Button_restart = new Sprite2D();
	UI_Button_restart->Init(ImagePath + "restart_button.png", 1, 1, 24, false, 0, 0);
	
	//	Debug axis
	DebugSprite = new Sprite2D();
	DebugSprite->Init(ImagePath + "axis.png", 1, 1, 24, false, 0, 0);

	return;
}

void InitParticle()
{
	ParticleSystem::InitShaderSystem(ShaderPath);
	ParticleSystem::InitSpriteTable(ImagePath);
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
	uniforms.fading = glGetUniformLocation(program, "fading");

	glUseProgram(program);
	///////////////////////////	

	//Load model to shader program
	My_LoadTextures();
	SetVaoVbo();

	//	init particle shader
	InitParticle();

	//	init window shader
	WindowShader::InitShaderProgram(ShaderPath);

	m_camera.ToggleOrtho();
	m_camera.Zoom(64);

	// will
	myGameState = new GameState();
}

//	Draw a single animation (character)
void DrawAnimation(Animation* anim, glm::mat4 _matrix, float fading = 0.0f)
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
	///	anchor�վ��:
	///glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix() * _matrix * translate(0, debug_x, 0) * anim->spriteSheet->GetModelMat()));
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
	glUniform1f(uniforms.fading, fading);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	anim->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

//	draw only one-frame sprite
void DrawSprite(Sprite2D* _sprite, glm::mat4 _parentMatrix, const vec3& _position, const vec3& _scale, float fading = 0.0f)
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
	glUniform1f(uniforms.fading, fading);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	_sprite->Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

void ResetGameState()
{
	isGameOver = false;
	delete myGameState;
	myGameState = NULL;
	myGameState = new GameState();
	cout << "Reset Game" << endl;
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	WindowShader::BindFrameBuffer();

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////	Draw background		//////////////////////////////////////////////
	
	DrawSprite(BackgroundSprite, translate(0, 0, 0), vec3(0, 0, 0), vec3(9, 9, 1));


	////////////////	Draw shadows		//////////////////////////////////////////////
	//mat4 parentMatrix = translate(0, 0, 0) * scale(1, 1, 1);
	mat4 parentMatrix = animations[CharacterIndex]->anchorTranslate;
	DrawSprite(ShadowSprite, parentMatrix, vec3(animations[CharacterIndex]->shadowOffsetX, animations[CharacterIndex]->shadowOffsetY, 0), vec3(animations[CharacterIndex]->shadowScale, animations[CharacterIndex]->shadowScale, 1));
	if (myGameState != NULL)
	{
		for (int i = GameObject::actors.size() - 1; i >= 0; i--)
		{
			GameObject* go = GameObject::actors[i];
			// draw
			mat4 trans = translate(go->position, go->distance * GameObject::depthRatio, 0)
				* scale(go->scale * go->facing, go->scale, 1);
			DrawSprite(
				ShadowSprite,
				trans * go->sprite->anchorTranslate,
				vec3(go->sprite->shadowOffsetX,
					go->sprite->shadowOffsetY, 0),
				vec3(go->sprite->shadowScale,
					go->sprite->shadowScale, 1),
				go->fading
			);
		}
	}

	////////////////	Draw characters		//////////////////////////////////////////////

	DrawAnimation(animations[CharacterIndex], parentMatrix);
	if (myGameState != NULL)
	{
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
			DrawAnimation(go->sprite, trans * go->sprite->anchorTranslate, go->fading);
		}
	}

	////////////////	Draw foreground		//////////////////////////////////////////////

	ParticleSystem::RenderInstances(m_camera, aspect);

	////////////////	Draw UI				//////////////////////////////////////////////

	// draw character buttons
	for (int i = 0; i < CHARNUM; i++)
	{
		DrawSprite(UI_Button_chara_back, translate(0, 0, 0), vec3(UI_trans[i][0], UI_trans[i][1], 0), vec3(UI_trans[i][2], UI_trans[i][2], 1));
		DrawSprite(UI_Button_chara_frame, translate(0, 0, 0), vec3(UI_trans[i][0], UI_trans[i][1], 0), vec3(UI_trans[i][2], UI_trans[i][2], 1));
	}
	for (int i = 0; i < CHARNUM; i++)
	{
		DrawAnimation(UI_Button_chara[i], translate(UI_trans[i][0], UI_trans[i][1] - UI_Button_chara_size, 0) * scale(-character_scale[i], character_scale[i], 1) * UI_Button_chara[i]->anchorTranslate);
	}

	// draw money lv up button UI
	DrawSprite(UI_Button_money, translate(0, 0, 0), vec3(UI_trans[5][0], UI_trans[5][1], 0), vec3(UI_trans[5][2], UI_trans[5][2], 1));
	// darw laser button UI

	if (isGameOver)
	{
		DrawSprite(UI_black, translate(0, 0, 0), vec3(UI_trans[7][0], UI_trans[7][1], 0), vec3(UI_trans[7][2], UI_trans[7][2], 1));
		if (isWin)
		{
			DrawSprite(UI_victory_title, translate(0, 0, 0), vec3(UI_trans[8][0], UI_trans[8][1], 0), vec3(UI_trans[8][2], UI_trans[8][2], 1));
		}
		else
		{
			DrawSprite(UI_defeat_title, translate(0, 0, 0), vec3(UI_trans[9][0], UI_trans[9][1], 0), vec3(UI_trans[9][2], UI_trans[9][2], 1));
		}
		DrawSprite(UI_Button_restart, translate(0, 0, 0), vec3(UI_trans[10][0], UI_trans[10][1], 0), vec3(UI_trans[10][2], UI_trans[10][2], 1));
	}

	if (DEBUG_MODE)
	{
		//	Debug axis
		DrawSprite(DebugSprite, translate(0, 0, 0), vec3(0, 0, 0), vec3(4, 4, 1));
		DrawSprite(DebugSprite, translate(1, 1, 0), vec3(0, 0, 0), vec3(4, 4, 1));
	}

	
	////////////////	Post FX				//////////////////////////////////////////////
	WindowShader::Render();

	glutSwapBuffers();
}


//Call to resize the window
void My_Reshape(int width, int height)
{
	camara_shape[0] = width;
	camara_shape[1] = height;
	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);

	//	for frame shader
	WindowShader::ChangeSize(width, height);
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
	// gameobjects
	if (myGameState != NULL)
	{
		myGameState->Update(((float)UPDATE_CYCLE) * updateSpeed);
	}
	// UIs
	for (int i = 0; i < CHARNUM; i++)
	{
		UI_Button_chara[i]->Elapse(((float)UPDATE_CYCLE) * updateSpeed);
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
			UIButton(x, y);
			// printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
		/*else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}*/
	}
	/*else if (button == GLUT_RIGHT_BUTTON)
	{
		printf("Mouse %d is pressed\n", button);
	}
	printf("%d %d %d %d\n", button, state, x, y);*/
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
	///m_camera.keyEvents(key);
	//printf("Key %c is pressed at (%d, %d)\n", key, x, y);

	switch (key)
	{
	case 'w':
		ParticleSystem::CreateInstance(50, "Hit", vec2(0, 6.28f), vec2(-0.001f, -1.5f),
			vec2(0.0f, 0.5f), 1.8f, 0.5f,
			vec2(0, 6.28f), vec2(0.3f, 0.4f), 0.3, 1.0f);
		/*	parameters:
									  ( _amount, _spriteName, _directionLH, _speedLH,
			_spawnRadiusLH,	_fadeRadius, _fadeDistance,
			_rotationLH, _scaleLH, _lifetime, _timeSpeed)
		*/

		debug_y += 0.2f;
		cout << "debug_y = " << debug_y << "\n";
		break;
	case 's':
		
		debug_y -= 0.2f;
		cout << "debug_y = " << debug_y << "\n";
		break;
	case 'd':
		debug_x += 0.05f;
		cout << "debug_x = " << debug_x << "\n";
		break;
	case 'a':
		debug_x -= 0.05f;
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
	case '6':
		myGameState->AddBattler(0, true);
		break;
	case '7':
		myGameState->AddBattler(1, true);
		break;
	case '8':
		myGameState->AddBattler("L_Tank", false);
		break;
	case '9':
		myGameState->AddBattler("L_Ranger", false);
		break;
	case '0':
		myGameState->enableEnemyAI = !myGameState->enableEnemyAI;
		cout << "Game: AI " << ((myGameState->enableEnemyAI) ? "Enabled" : "Disabled") << endl;
		break;
	case 'r':
		ResetGameState();
		break;
	case 'l':
		myGameState->Laser();
		break;
	case 'm':
		cout << "Game: money: " << (myGameState->currentMoney) << "/" << myGameState->maxMoney_level[myGameState->currentLevel] << endl;
		break;
	case 'u':
		myGameState->LevelUp();
		break;
	case '=':
		DEBUG_MODE = !DEBUG_MODE;
		cout << "DEBUG_MODE = " << DEBUG_MODE << "\n";
		break;
	default:
		break;
	}
}

void My_Mouse_Moving(int x, int y) {
	m_camera.mouseMoveEvent(x, y);
}

void UIButton(float x, float y)
{
	// define mvp transform matrix
	mat4 modelToCam = m_camera.GetProjectionMatrix(aspect) * m_camera.GetViewMatrix() * m_camera.GetModelMatrix()
		* translate(0, 0, 0);
	
	if (!isGameOver)
	{
		float range = ((modelToCam * vec4(UI_Button_chara_size, 0, 0, 1)).x / 2) * camara_shape[0] * 1.5f;
		float range_money = ((modelToCam * vec4(UI_Button_money_size, 0, 0, 1)).x / 2) * camara_shape[0] * 1.5f;
		// character button
		for (int i = 0; i < CHARNUM; i++)
		{
			vec4 camPos = modelToCam * vec4(UI_trans[i][0], UI_trans[i][1], 0, 1);
			float UI_pos[2] =
			{
				(camPos.x / 2 + 0.5f) * camara_shape[0],
				(-camPos.y / 2 + 0.5f) * camara_shape[1]
			};
			if (sqrtf(pow(x - UI_pos[0], 2) + pow(y - UI_pos[1], 2)) < range)
			{
				// cout << "press character button " << i << endl;
				myGameState->AddBattler(character_names[i], false);
				return;
			}
		}

		// money button
		{
			vec4 camPos = modelToCam * vec4(UI_trans[5][0], UI_trans[5][1], 0, 1);
			float UI_pos[2] =
			{
				(camPos.x / 2 + 0.5f) * camara_shape[0],
				(-camPos.y / 2 + 0.5f) * camara_shape[1]
			};
			if (sqrtf(pow(x - UI_pos[0], 2) + pow(y - UI_pos[1], 2)) < range_money)
			{
				myGameState->LevelUp();
				return;
			}
		}
	}
	else
	{
		float restart_size[2] = 
		{
			((modelToCam * vec4(UI_Button_restart_size[0], 0, 0, 1)).x / 2) * camara_shape[0] * 1.5f,
			((modelToCam * vec4(UI_Button_restart_size[1], 0, 0, 1)).x / 2) * camara_shape[0] * 1.5f
		};
		vec4 camPos = modelToCam * vec4(UI_trans[10][0], UI_trans[10][1], 0, 1);
		float UI_pos[2] =
		{
			(camPos.x / 2 + 0.5f) * camara_shape[0],
			(-camPos.y / 2 + 0.5f) * camara_shape[1]
		};
		if ((abs(x - UI_pos[0]) < restart_size[0] / 2) && (abs(y - UI_pos[1]) < restart_size[1] / 2))
		{
			// restart
			ResetGameState();
			return;
		}
	}

}
void GameOverTitle(bool win)
{
	cout << "Game: Calls game over title: " << (win? "victory" : "defeat") << endl;
	isGameOver = true;
	isWin = win;
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

