#include "GL/glew.h"
#include "GL/freeglut.h"

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "XML_Reader.h"
#include "OBJ_Reader.h"

/* Graphic Objects */
#include "Object.h"
#include "JointObject.h"

/* Post Processing */
#include "FrameBuffer.h"
#include "PostProcessing.h"

/* Lighting */
#include "SpotLight.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"

/* Shaders */
#include "BlinnPhongShader.h"
#include "MixedTextureShader.h"
#include "BumpMapShader.h"
#include "RealWoodShader.h"
#include "SphereMapShader.h"
#include "CubeMapShader.h"

/* Textures */
#include "Texture.h"
#include "GeneratedTexture.h"

/* Scene Manager */
#include "SceneManager.h"

#define FPS_60	1000/60

#define CAPTION	"P3DSJ 2013"

/* Frame Count Global Variable */
int frameCount = 0;

/* Window Handling Global Variables */
int windowHandle = 0;

int windowWidth = WIDTH;
int windowHeight = HEIGHT;

/* Clock Handling Global Variables */
GLint startTime = 0;
GLfloat time = 0;
GLfloat elapsedTime = 0;

/* Scene Manager */
SceneManager* sceneManager = SceneManager::getInstance();

FrameBuffer* framebuffer;
PostProcessing* postProcessing;

void soilTest() {

	GLuint texture = SOIL_load_OGL_texture("textures/benfica-2x2.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	/* check for an error during the load process */
	if(texture == 0)
		cout << "SOIL loading error: " << SOIL_last_result() << endl;

	Utility::checkOpenGLError("ERROR: Texture loading failed.");

	GLint width;
	GLint height;
	GLint channels;

	unsigned char* heightMap = SOIL_load_image("textures/benfica.bmp",&width,&height, &channels,SOIL_LOAD_AUTO);

	GLuint saveResult = SOIL_save_image("textures/benfica_saved.bmp",SOIL_SAVE_TYPE_BMP,width,height,channels,heightMap);

	/* check for an error during the load process */
	if(saveResult == 0)
		cout << "SOIL saving error: " << SOIL_last_result() << endl;
	
	/* save a screenshot of your awesome OpenGL game engine, running at 1024x768 */
	GLuint screenshotResult = SOIL_save_screenshot("textures/screenshot.bmp", SOIL_SAVE_TYPE_BMP, 0, 0, windowWidth, windowHeight);

	/* check for an error during the load process */
	if(screenshotResult == 0)
		cout << "SOIL saving error: " << SOIL_last_result() << endl;

	SOIL_free_image_data(heightMap);
}

void cleanup() {

	sceneManager->destroyInstance();

	//framebuffer->destroyInstance();
	//postProcessing->destroyInstance();
}

void update(int value) {

	elapsedTime = (GLfloat)(glutGet(GLUT_ELAPSED_TIME) - startTime)/1000;
	startTime = glutGet(GLUT_ELAPSED_TIME);
	time += elapsedTime;

	sceneManager->update(elapsedTime);

	glutTimerFunc(FPS_60, update, 0);

	glutPostRedisplay();
}

void display() {

	++frameCount;

	/* Draw to the PostProcessing Buffer */
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFrameBufferObject());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager->draw();

	//glBindFramebuffer(GL_FRAMEBUFFER,0);

	/* Draw to the Screen */
	//postProcessing->draw(framebuffer);

	glutSwapBuffers();
}

void reshape(int w, int h) {

	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, windowWidth, windowHeight);

	sceneManager->reshape(windowWidth,windowHeight);

	framebuffer->reshape(windowWidth,windowHeight);
	postProcessing->reshape(framebuffer);
}

void timer(int value) {

	std::ostringstream oss;
	oss << CAPTION << ": " << frameCount << " FPS @ (" << windowWidth << "x" << windowHeight << ")";
	std::string s = oss.str();

	glutSetWindow(windowHandle);
	glutSetWindowTitle(s.c_str());

    frameCount = 0;

    glutTimerFunc(1000, timer, 0);
}

void normalKeyListener(unsigned char key, int x, int y) {

	KeyboardHandler::getInstance()->normalKeyListener(key,x,y);
}

void releasedNormalKeyListener(unsigned char key, int x, int y) {

	KeyboardHandler::getInstance()->releasedNormalKeyListener(key,x,y);
}

void specialKeyListener(int key, int x, int y) {

	KeyboardHandler::getInstance()->specialKeyListener(key,x,y);
}

void releasedSpecialKeyListener(int key, int x, int y) {

	KeyboardHandler::getInstance()->releasedSpecialKeyListener(key,x,y);
}

void mouseEventListener(int button, int state, int x, int y) {

	MouseHandler::getInstance()->mouseEventListener(button,state,x,y);
}

void mouseMovementListener(int x, int y) {

	MouseHandler::getInstance()->mouseMovementListener(x,y);
}

void mousePassiveMovementListener(int x, int y) {

	MouseHandler::getInstance()->mouseMovementListener(x,y);
}

void mouseWheelListener(int button, int direction, int x, int y)  {

	MouseHandler::getInstance()->mouseWheelListener(button,direction,x,y);
} 

void setupCallbacks() {

	glutCloseFunc(cleanup);

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(normalKeyListener); 
	glutKeyboardUpFunc(releasedNormalKeyListener); 
	glutSpecialFunc(specialKeyListener);
	glutSpecialUpFunc(releasedSpecialKeyListener);

	glutMouseFunc(mouseEventListener);
	glutMotionFunc(mouseMovementListener);
	glutPassiveMotionFunc(mousePassiveMovementListener);
	glutMouseWheelFunc(mouseWheelListener);

	glutTimerFunc(0,timer,0);
	glutTimerFunc(0,update,0);
}

void setupOpenGL() {

	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0,1.0);
	glClearDepth(1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {

	glewExperimental = GL_TRUE;

	GLenum result = glewInit(); 

	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 

	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[]) {

	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow(CAPTION);

	if(windowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void setupShaders() {

	/* Create Blinn Phong Shader */
	BlinnPhongShader* blinnPhongShader = new BlinnPhongShader(BLINN_PHONG_SHADER);
	blinnPhongShader->createShaderProgram();
	blinnPhongShader->bindAttributes();
	blinnPhongShader->linkShaderProgram();
	blinnPhongShader->bindUniforms();

	sceneManager->addShaderProgram(blinnPhongShader);

	/* Create Mixed Texture Map Shader*/
	MixedTextureShader* mixedTextureShader = new MixedTextureShader(MIXED_TEXTURE_SHADER);
	mixedTextureShader->createShaderProgram();
	mixedTextureShader->bindAttributes();
	mixedTextureShader->linkShaderProgram();
	mixedTextureShader->bindUniforms();

	sceneManager->addShaderProgram(mixedTextureShader);

	/* Create Bump Map Shader*/
	BumpMapShader* bumpMapShader = new BumpMapShader(BUMPMAP_SHADER);
	bumpMapShader->createShaderProgram();
	bumpMapShader->bindAttributes();
	bumpMapShader->linkShaderProgram();
	bumpMapShader->bindUniforms();

	sceneManager->addShaderProgram(bumpMapShader);

	/* Create Sphere Map Shader*/
	SphereMapShader* sphereMapShader = new SphereMapShader(SPHERE_MAP_SHADER);
	sphereMapShader->createShaderProgram();
	sphereMapShader->bindAttributes();
	sphereMapShader->linkShaderProgram();
	sphereMapShader->bindUniforms();

	sceneManager->addShaderProgram(sphereMapShader);

	/* Create Cube Map Shader*/
	CubeMapShader* cubeMapShader = new CubeMapShader(CUBE_MAP_SHADER);
	cubeMapShader->createShaderProgram();
	cubeMapShader->bindAttributes();
	cubeMapShader->linkShaderProgram();
	cubeMapShader->bindUniforms();

	sceneManager->addShaderProgram(cubeMapShader);

	/* Create Real Wood Shader * /
	RealWoodShader* realWoodShader = new RealWoodShader(REAL_WOOD_SHADER);
	realWoodShader->createShaderProgram();
	realWoodShader->bindAttributes();
	realWoodShader->linkShaderProgram();
	realWoodShader->bindUniforms();

	sceneManager->addShaderProgram(realWoodShader);*/

	/* Set Active Shader */
	sceneManager->setActiveShaderProgram(blinnPhongShader);
}

void setupLights() {

	/* Light Source 0 */
	DirectionalLight* directionalLight0 = new DirectionalLight(DIRECTIONAL_LIGHT_0);

	directionalLight0->setIdentifier(LIGHT_SOURCE_0);

	directionalLight0->setDirection(Vector(0.0f, 1.0f, 0.0f, 1.0f));
	directionalLight0->setColor(Vector(0.0f, 0.0f, 1.0f, 1.0f));

	directionalLight0->setAmbientIntensity(0.05f);
	directionalLight0->setDiffuseIntensity(1.0f);
	directionalLight0->setSpecularIntensity(1.0f);

	sceneManager->addLight(directionalLight0);

	/* Light Source 1 */
	PositionalLight* positionalLight1 = new PositionalLight(POSITIONAL_LIGHT_1);

	positionalLight1->setIdentifier(LIGHT_SOURCE_1);

	positionalLight1->setPosition(Vector(0.0f, 10.0f, 0.0f, 1.0f));
	positionalLight1->setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));

	positionalLight1->setAmbientIntensity(0.25f);
	positionalLight1->setDiffuseIntensity(0.95f);
	positionalLight1->setSpecularIntensity(0.95f);

	positionalLight1->setConstantAttenuation(0.5f);
	positionalLight1->setLinearAttenuation(0.000675f);
	positionalLight1->setExponentialAttenuation(0.00025f);

	sceneManager->addLight(positionalLight1);

	/* Light Source 2 */
	SpotLight* spotLight2 = new SpotLight(SPOT_LIGHT_2);

	spotLight2->setIdentifier(LIGHT_SOURCE_2);

	spotLight2->setPosition(Vector(0.0f, 5.0f, 0.0f, 1.0f));
	spotLight2->setDirection(Vector(0.0f, -1.0f, 0.0f, 1.0f));
	spotLight2->setColor(Vector(1.0f, 0.0f, 0.0f, 1.0f));

	spotLight2->setCutOff(cos(25.0f * (GLfloat)DEGREES_TO_RADIANS));

	spotLight2->setAmbientIntensity(0.0f);
	spotLight2->setDiffuseIntensity(0.95f);
	spotLight2->setSpecularIntensity(0.95f);

	spotLight2->setConstantAttenuation(0.5f);
	spotLight2->setLinearAttenuation(0.00675f);
	spotLight2->setExponentialAttenuation(0.0025f);

	//sceneManager->addLight(spotLight2);
}

void setupCameras() {

	/* Create Orthogonal Camera */
	Camera* orthogonalCamera = new Camera(ORTHOGONAL_NAME);

	orthogonalCamera->loadOrthogonalProjection();
	orthogonalCamera->loadView();

	sceneManager->addCamera(orthogonalCamera);

	/* Create Perspective Camera */
	Camera* perspectiveCamera = new Camera(PERSPECTIVE_NAME);
	perspectiveCamera->setPosition(Vector(-5.0f, 2.5f, 0.0f,1.0f));
	perspectiveCamera->loadPerspectiveProjection();
	perspectiveCamera->loadView();

	sceneManager->addCamera(perspectiveCamera);

	/* Set Active Camera */
	sceneManager->setActiveCamera(perspectiveCamera);
}

void init(int argc, char* argv[]) {

	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	//freopen("output.txt","w",stderr);
	//freopen("output.txt","w",stdout);

	setupShaders();

	setupLights();

	setupCameras();

	/* Parse Models and Transformations */
	OBJ_Reader* objReader = OBJ_Reader::getInstance();
	XML_Reader* xmlReader = XML_Reader::getInstance();

	xmlReader->openTransformationFile(TRANSFORMATION_FILE);

	/* Table * /
	Object* table = new Object(TABLE);
	table->activateWoodTexture(1.0f,2.0f,4,0.16f);

	objReader->loadModel("Table.obj","Table.mtl", table);
	xmlReader->loadTransformation(table);

	sceneManager->addObject(table);
	*/

	/* Table Surface */
	Object* tableSurface = new Object(TABLE_SURFACE);
	tableSurface->activateMixedTexture("textures/tampo_diffuse.png","textures/tampo_diffuse.png");

	objReader->loadModel("TableSurface.obj","TableSurface.mtl", tableSurface);
	xmlReader->loadTransformation(tableSurface);

	sceneManager->addObject(tableSurface);

	/* Blinn-Phong */
	Object* blinnPhongObject = new Object(BLINN_PHONG_OBJECT);
	blinnPhongObject->setPosition(Vector(-10.0f,2.5f,0.0f,1.0f));
	blinnPhongObject->setRotation(Vector(0.0f,90.0f,0.0f,1.0f));
	blinnPhongObject->activateBumpTexture("textures/fieldstone_diffuse.jpg","textures/fieldstone_normal.jpg");

	objReader->loadModel("Torus.obj","Torus.mtl", blinnPhongObject);

	sceneManager->addObject(blinnPhongObject);

	/* Bump Mapping */
	Object* bumpMappingObject = new Object(BUMP_MAPPING_OBJECT);
	bumpMappingObject->setPosition(Vector(10.0f,2.5f,0.0f,1.0f));
	bumpMappingObject->setRotation(Vector(0.0f,90.0f,0.0f,1.0f));
	bumpMappingObject->setScale(Vector(0.25f,0.25f,0.25f,1.0f));
	bumpMappingObject->activateBumpTexture("textures/fieldstone_diffuse.jpg","textures/fieldstone_normal.jpg");

	objReader->loadModel("teapot/teapot2.obj","teapot/GoldTeapot.mtl", bumpMappingObject);

	sceneManager->addObject(bumpMappingObject);

	/* Sphere Environmental Mapping */
	Object* sphereEnvironmentalMappingObject = new Object(SPHERE_MAPPING_OBJECT);
	sphereEnvironmentalMappingObject->setPosition(Vector(0.0f, 2.5f,-10.0f,1.0f));
	//sphereEnvironmentalMappingObject->setScale(Vector(0.15f,0.15f,0.15f,1.0f));
	sphereEnvironmentalMappingObject->setScale(Vector(2.5f,2.5f,2.5f,1.0f));
	sphereEnvironmentalMappingObject->activateSphereMapTexture("textures/SphereMap.jpg");

	objReader->loadModel("Sphere.obj","Sphere.mtl", sphereEnvironmentalMappingObject);
	//objReader->loadModel("teapot/teapot2.obj","teapot/SilverTeapot.mtl", sphereEnvironmentalMappingObject);

	sceneManager->addObject(sphereEnvironmentalMappingObject);

	/* Cube  Environmental Mapping */
	Object* cubeEnvironmentalMappingObject = new Object(CUBE_MAPPING_OBJECT);
	cubeEnvironmentalMappingObject->setPosition(Vector(0.0f, 2.5f,10.0f,1.0f));
	cubeEnvironmentalMappingObject->setRotation(Vector(0.0f,180.0f,0.0f,1.0f));
	//cubeEnvironmentalMappingObject->setScale(Vector(0.15f,0.15f,0.15f,1.0f));
	cubeEnvironmentalMappingObject->setScale(Vector(2.5f,2.5f,2.5f,1.0f));
	cubeEnvironmentalMappingObject->activateCubeMapTexture(
		"textures/beach/posx.jpg","textures/beach/negx.jpg",
		"textures/beach/posy.jpg","textures/beach/negy.jpg",
		"textures/beach/posz.jpg","textures/beach/negz.jpg");	

	objReader->loadModel("Sphere.obj","Sphere.mtl", cubeEnvironmentalMappingObject);
	//objReader->loadModel("teapot/teapot2.obj","teapot/SilverTeapot.mtl", cubeEnvironmentalMappingObject);

	sceneManager->addObject(cubeEnvironmentalMappingObject);

	/* Destroy the Readers */
	OBJ_Reader::destroyInstance();
	XML_Reader::destroyInstance();

	/* Create Scene Graph Nodes */
	/*SceneNode* tableNode = new SceneNode(TABLE);
	tableNode->setObject(table);
	tableNode->setShaderProgram(sceneManager->getShaderProgram(REAL_WOOD_SHADER));*/

	SceneNode* tableSurfaceNode = new SceneNode(TABLE_SURFACE);
	tableSurfaceNode->setObject(tableSurface);
	tableSurfaceNode->setShaderProgram(sceneManager->getShaderProgram(MIXED_TEXTURE_SHADER));

	SceneNode* blinnPhongObjectNode = new SceneNode(BLINN_PHONG_OBJECT);
	blinnPhongObjectNode->setObject(blinnPhongObject);
	blinnPhongObjectNode->setShaderProgram(sceneManager->getShaderProgram(BLINN_PHONG_SHADER));

	SceneNode* bumpMappingObjectNode = new SceneNode(BUMP_MAPPING_OBJECT);
	bumpMappingObjectNode->setObject(bumpMappingObject);
	bumpMappingObjectNode->setShaderProgram(sceneManager->getShaderProgram(BUMPMAP_SHADER));

	SceneNode* sphereEnvironmentalMappingObjectNode = new SceneNode(SPHERE_MAPPING_OBJECT);
	sphereEnvironmentalMappingObjectNode->setObject(sphereEnvironmentalMappingObject);
	sphereEnvironmentalMappingObjectNode->setShaderProgram(sceneManager->getShaderProgram(SPHERE_MAP_SHADER));

	SceneNode* cubeEnvironmentalMappingObjectNode = new SceneNode(CUBE_MAPPING_OBJECT);
	cubeEnvironmentalMappingObjectNode->setObject(cubeEnvironmentalMappingObject);
	cubeEnvironmentalMappingObjectNode->setShaderProgram(sceneManager->getShaderProgram(CUBE_MAP_SHADER));

	/* Add the Root Nodes to the Scene */
	//sceneManager->addSceneNode(tableNode);
	sceneManager->addSceneNode(tableSurfaceNode);

	sceneManager->addSceneNode(blinnPhongObjectNode);
	sceneManager->addSceneNode(bumpMappingObjectNode);
	sceneManager->addSceneNode(sphereEnvironmentalMappingObjectNode);
	sceneManager->addSceneNode(cubeEnvironmentalMappingObjectNode);

	/* FMOD Sound Loading */
	Sound* arrowSound = new Sound(ARROW_SOUND_NAME,ARROW_SOUND_FILE);
	arrowSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(arrowSound);

	Sound* cannonSound = new Sound(CANNON_SOUND_NAME,CANNON_SOUND_FILE);
	cannonSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(cannonSound);

	Sound* punchSound = new Sound(PUNCH_SOUND_NAME,PUNCH_SOUND_FILE);
	punchSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(punchSound);

	Sound* sweeperSound = new Sound(SWEEPER_SOUND_NAME,SWEEPER_SOUND_FILE);
	sweeperSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(sweeperSound);

	Sound* musicSound = new Sound(MUSIC_SOUND_NAME,MUSIC_SOUND_FILE);
	musicSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(musicSound);

	Sound* rayquazaSound = new Sound(RAYQUAZA_SOUND_NAME,RAYQUAZA_SOUND_FILE);
	rayquazaSound->setMode(FMOD_LOOP_OFF);

	sceneManager->addSound(rayquazaSound);

	/* Init the SceneManager */
	sceneManager->init();

	/* Init the FrameBuffer */
	/*framebuffer = FrameBuffer::getInstance();
	framebuffer->init(windowWidth,windowHeight);*/

	/* Init the PostProcessing Shader */
	/*postProcessing = PostProcessing::getInstance();
	postProcessing->createShaderProgram();
	postProcessing->bindAttributes();
	postProcessing->linkShaderProgram();
	postProcessing->bindUniforms();*/

	/* Setup GLUT Callbacks */
	setupCallbacks();
}

int main(int argc, char* argv[]) {

	/* Init the Animation */
	init(argc, argv);

	/* Enable User Interaction */
	KeyboardHandler::getInstance()->enableKeyboard();
	MouseHandler::getInstance()->enableMouse();

	/* Start the Clock */
	startTime = glutGet(GLUT_ELAPSED_TIME);

	/* Start the Animation! */
	glutMainLoop();

	#ifdef MEMORY_LEAK
		_CrtDumpMemoryLeaks();
	#endif

	exit(EXIT_SUCCESS);
}