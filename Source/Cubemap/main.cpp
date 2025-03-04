#include <Common.h>
#include <ViewManager.h>
#include <ResourcePath.h>
#include "CubemapShader.h"
#include "DrawModelShader.h"
#include "MyModel.h"

using namespace glm;
using namespace std;

const std::string ProjectName = "CubeMap";

GLuint			program;		//shader program
mat4			projMat;		//projection matrix
mat4			viewMat;		//projection matrix
mat4			modelMat;		//projection matrix
float			aspect;
vec3			cameraPos(0, 0, 3);
		
GLuint			textureID;
float yawAngle = 0.0f;
float pitchAngle = 0.0f;
float lastMouseX;
float lastMouseY;
bool isLeftMousePressed = false;

CubemapShader cubemapShader;
DrawModelShader drawModelShader;
MyModel model;

//	second skybox
int skyboxIndex = 0;
bool testBool = false;
float refractRatio = 1.0f;

void My_Init()
{
	ResourcePath::shaderPath = "./Shader/" + ProjectName + "/";
	ResourcePath::imagePath = "./Imgs/" + ProjectName + "/";
	std::string imagePath2 = "./Imgs/" + ProjectName + "/cubemap2/";


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize shaders
	///////////////////////////
	std::vector<CubemapTexture> textures(6);
	textures[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures[0].fileName = ResourcePath::imagePath + "posx.jpg";
	textures[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures[1].fileName = ResourcePath::imagePath + "negx.jpg";
	textures[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures[2].fileName = ResourcePath::imagePath + "negy.jpg";
	textures[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures[3].fileName = ResourcePath::imagePath + "posy.jpg";
	textures[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures[4].fileName = ResourcePath::imagePath + "posz.jpg";
	textures[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures[5].fileName = ResourcePath::imagePath + "negz.jpg";

	std::vector<CubemapTexture> textures2(6);
	textures2[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures2[0].fileName = imagePath2 + "posx.jpg";
	textures2[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures2[1].fileName = imagePath2 + "negx.jpg";
	textures2[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures2[2].fileName = imagePath2 + "negy.jpg";
	textures2[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures2[3].fileName = imagePath2 + "posy.jpg";
	textures2[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures2[4].fileName = imagePath2 + "posz.jpg";
	textures2[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures2[5].fileName = imagePath2 + "negz.jpg";

	cubemapShader.Init(textures, textures2);
	drawModelShader.Init();
	model.Init("Model/Potion_bottle.obj");

	viewMat = lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glm::mat4 rotMat;
	
	rotMat = glm::rotate(rotMat, radians(pitchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	rotMat = glm::rotate(rotMat, radians(yawAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 newCameraPos = cameraPos * glm::mat3(rotMat);
	viewMat = lookAt(newCameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 mvMat = viewMat * modelMat;
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cubemapShader.Enable();
	cubemapShader.SetMVMat(glm::mat4(glm::mat3(viewMat)));
	cubemapShader.SetPMat(projMat);
	cubemapShader.Render(skyboxIndex);
	cubemapShader.Disable();

	drawModelShader.Enable();
	drawModelShader.SetMMat(modelMat);
	drawModelShader.SetMVMat(mvMat);
	drawModelShader.SetPMat(projMat);
	drawModelShader.SetNormalMat(normalMat);
	drawModelShader.SetCameraPos(newCameraPos);
	drawModelShader.SetRefractRatio(refractRatio / 1.52f);
	drawModelShader.SetDrawMode(testBool);
	cubemapShader.UseTexture(true, skyboxIndex);
	model.Render();
	cubemapShader.UseTexture(false, skyboxIndex);
	drawModelShader.Disable();

    glutSwapBuffers();
}

//Call to resize the window
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	glViewport(0, 0, width, height);
	projMat = perspective(radians(60.0f), aspect, 0.1f, 1000.0f);
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
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			isLeftMousePressed = true;
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			isLeftMousePressed = false;
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		
		if (state == GLUT_DOWN)
		{
			skyboxIndex = (skyboxIndex + 1) % 2;
		}
			
		printf("Mouse %d is pressed\n", button);
	}
	printf("%d %d %d %d\n",button,state,x,y);
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{

	if (key == 'r')
	{
		refractRatio += 0.1f;
		if (refractRatio >= 1.0f) refractRatio = 0;
	}
	if (key == 'z')
	{
		testBool = !testBool;
	}
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
}


void My_Mouse_Moving(int x, int y) {

	if (isLeftMousePressed)
	{
		yawAngle += 0.1f * (x - lastMouseX);
		pitchAngle += 0.1f * (y - lastMouseY);
	}

	lastMouseX = x;
	lastMouseY = y;

}

int main(int argc, char *argv[])
{
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
	glutInitWindowSize(800, 800);
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

	return 0;
}

