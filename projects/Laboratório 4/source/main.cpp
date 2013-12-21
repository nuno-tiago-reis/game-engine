#include "GL/glew.h"
#include "GL/freeglut.h"
#include "tinyxml2.h"

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "XML_Editor.h"
#include "OBJ_Reader.h"

/* Graphic Objects */
#include "Pin.h"
#include "Puck.h"
#include "Mallet.h"
#include "TableWall.h"

/* Post Processing */
#include "FrameBuffer.h"
#include "PostProcessing.h"

/* Lighting */
#include "LightingManager.h"
#include "SpotLight.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"

/* Shaders */
#include "SilhouetteShader.h"
#include "BlinnPhongShader.h"
#include "BumpMapShader.h"
#include "RealWoodShader.h"
#include "Texture.h"
#include "GeneratedTexture.h"

#include "SceneManager.h"

#define FPS_60	1000/60

#define CAPTION	"AVT 2013"

#define POST_PROC_VERTEX_SHADER_FILE		"shaders/post_proc_VertexShader.glsl"
#define POST_PROC_FRAGMENT_SHADER_FILE		"shaders/post_proc_FragmentShader.glsl"

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

LightingManager* lightingManager = LightingManager::getInstance();

FrameBuffer* framebuffer;
PostProcessing* postprocessing;

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

	SceneManager::destroyInstance();

	XML_Editor::destroyInstance();

	framebuffer->destroy();
	delete postprocessing;
}

void update(int value) {

	elapsedTime = (GLfloat)(glutGet(GLUT_ELAPSED_TIME) - startTime)/1000;
	startTime = glutGet(GLUT_ELAPSED_TIME);
	time += elapsedTime;
	glutTimerFunc(FPS_60, update, 0);
	sceneManager->update(elapsedTime);

	glutPostRedisplay();

	
}

void display() {

	++frameCount;

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFBO());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager->draw();

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	postprocessing->draw(framebuffer);

	glutSwapBuffers();
}

void reshape(int w, int h) {

	windowWidth = w;
	windowHeight = h;

	glViewport(0, 0, windowWidth, windowHeight);

	sceneManager->reshape(windowWidth,windowHeight);

	framebuffer->onReshape(windowWidth,windowHeight);
	postprocessing->onCameraReset();
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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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

void init(int argc, char* argv[]) {

	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	/* Create Bump Map Shader*/
	BumpMapShader* bumpMapShader = new BumpMapShader(BUMPMAP_SHADER);
	bumpMapShader->createShaderProgram();
	bumpMapShader->bindAttributes();
	bumpMapShader->linkShaderProgram();
	bumpMapShader->bindUniforms();

	sceneManager->addShaderProgram(bumpMapShader);

	/* Create Silhoette Shader */
	SilhouetteShader* silhouetteShader = new SilhouetteShader(SILHOUETTE_SHADER);
	silhouetteShader->createShaderProgram();
	silhouetteShader->bindAttributes();
	silhouetteShader->linkShaderProgram();
	silhouetteShader->bindUniforms();

	sceneManager->addShaderProgram(silhouetteShader);

	/* Create Blinn Phong Shader */
	BlinnPhongShader* blinnPhongShader = new BlinnPhongShader(BLINN_PHONG_SHADER);
	blinnPhongShader->createShaderProgram();
	blinnPhongShader->bindAttributes();
	blinnPhongShader->linkShaderProgram();
	blinnPhongShader->bindUniforms();

	sceneManager->addShaderProgram(blinnPhongShader);
	
	/* Create Real Wood Shader */
	RealWoodShader* realWoodShader = new RealWoodShader(REAL_WOOD_SHADER);
	realWoodShader->createShaderProgram();
	realWoodShader->bindAttributes();
	realWoodShader->linkShaderProgram();
	realWoodShader->bindUniforms();

	sceneManager->addShaderProgram(realWoodShader);


	/* Set Active Shader */
	sceneManager->setActiveShaderProgram(blinnPhongShader);

	/* Set Light properties */

	/* Light Source 0 */
	PositionalLight* positionalLight0 = new PositionalLight(POSITIONAL_LIGHT_0);

	positionalLight0->setIdentifier(LIGHT_SOURCE_0);

	positionalLight0->setPosition(Vector(0.0f,-10.0f, 10.0f, 1.0f));
	positionalLight0->setColor(Vector(1.0f, 0.0f, 0.0f, 1.0f));

	positionalLight0->setAmbientIntensity(0.2f);
	positionalLight0->setDiffuseIntensity(0.25f);
	positionalLight0->setSpecularIntensity(0.25f);

	positionalLight0->setConstantAttenuation(1.0f);
	positionalLight0->setLinearAttenuation(0.0f);
	positionalLight0->setExponentialAttenuation(0.0f);

	lightingManager->addPositionalLight(positionalLight0);

	/* Light Source 1 */
	PositionalLight* positionalLight1 = new PositionalLight(POSITIONAL_LIGHT_1);

	positionalLight1->setIdentifier(LIGHT_SOURCE_1);

	positionalLight1->setPosition(Vector(0.0f, 10.0f, 10.0f, 1.0f));
	positionalLight1->setColor(Vector(0.0f, 1.0f, 0.0f, 1.0f));

	positionalLight1->setAmbientIntensity(0.2f);
	positionalLight1->setDiffuseIntensity(0.25f);
	positionalLight1->setSpecularIntensity(0.25f);

	positionalLight1->setConstantAttenuation(1.0f);
	positionalLight1->setLinearAttenuation(0.0f);
	positionalLight1->setExponentialAttenuation(0.0f);

	lightingManager->addPositionalLight(positionalLight1);

	/* Light Source 2 */
	PositionalLight* positionalLight2 = new PositionalLight(POSITIONAL_LIGHT_2);

	positionalLight2->setIdentifier(LIGHT_SOURCE_2);

	positionalLight2->setPosition(Vector(0.0f, 0.0f, 10.0f, 1.0f));
	positionalLight2->setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));

	positionalLight2->setAmbientIntensity(0.2f);
	positionalLight2->setDiffuseIntensity(0.25f);
	positionalLight2->setSpecularIntensity(0.25f);

	positionalLight2->setConstantAttenuation(1.0f);
	positionalLight2->setLinearAttenuation(0.0f);
	positionalLight2->setExponentialAttenuation(0.0f);

	lightingManager->addPositionalLight(positionalLight2);

	/* Create Orthogonal Camera */
	Camera* orthogonalCamera = new Camera(ORTHOGONAL_NAME);

	orthogonalCamera->loadOrthogonalProjection();
	orthogonalCamera->loadView();

	sceneManager->addCamera(orthogonalCamera);

	/* Create Perspective Camera */
	Camera* perspectiveCamera = new Camera(PERSPECTIVE_NAME);

	perspectiveCamera->loadPerspectiveProjection();
	perspectiveCamera->loadView();

	sceneManager->addCamera(perspectiveCamera);

	/* Set Active Camera */
	sceneManager->setActiveCamera(perspectiveCamera);

	/* Parse Models and Transformations */
	XML_Editor* editor = XML_Editor::getInstance();

	editor->openTransformationFile(TRANSFORMATION_FILE);

	/* Table */
	GraphicObject* table = new GraphicObject(TABLE);

	sceneManager->addGraphicObject(table);
	OBJ_Reader::readFile("table", table);
	table->activateWood(1.0f,2.0f,4,0.16f);
	editor->loadTransformation(table);

	/* Table Surface */
	GraphicObject* tableSurface = new GraphicObject(TABLE_SURFACE);

	sceneManager->addGraphicObject(tableSurface);
	OBJ_Reader::readFile("table_surface", tableSurface);
	tableSurface->activateBump("textures/tampo_diffuse.png","textures/tampo_bump.png");
	editor->loadTransformation(tableSurface);

	/* Mallet */
	Mallet* mallet = new Mallet(MALLET);

	sceneManager->addGraphicObject(mallet);
	OBJ_Reader::readFile("mallet", mallet);
	mallet->activateBump("textures/mallet_diffuse.png","textures/mallet_bump.png");
	editor->loadTransformation(mallet);

	/* Puck */
	Puck* puck = new Puck(PUCK);
	sceneManager->addGraphicObject(puck);
	OBJ_Reader::readFile("disk", puck);
	puck->activateBump("textures/puck_diffuse.png","textures/puck_bump.png");
	editor->loadTransformation(puck);

	/* Pin A */
	Pin* pinA = new Pin(PIN_A);

	sceneManager->addGraphicObject(pinA);
	OBJ_Reader::readFile("testcircle", pinA);
	pinA->activateWood(1,1,4,0.3);
	editor->loadTransformation(pinA);

	/* Pin B */
	Pin* pinB = new Pin(PIN_B);

	sceneManager->addGraphicObject(pinB);
	OBJ_Reader::readFile("testcircle", pinB);
	pinB->activateWood(1,1,4,0.3);
	editor->loadTransformation(pinB);

	/* Pin C */
	Pin* pinC = new Pin(PIN_C);

	sceneManager->addGraphicObject(pinC);
	OBJ_Reader::readFile("testcircle", pinC);
	pinC->activateWood(1,1,4,0.3);
	editor->loadTransformation(pinC);

	/* TableWall A */
	TableWall* tableWallA = new TableWall(WALL_A);

	sceneManager->addGraphicObject(tableWallA);
	OBJ_Reader::readFile("testcube", tableWallA);
	editor->loadTransformation(tableWallA);

	/* TableWall B */
	TableWall* tableWallB = new TableWall(WALL_B);

	sceneManager->addGraphicObject(tableWallB);
	OBJ_Reader::readFile("testcube", tableWallB);
	editor->loadTransformation(tableWallB);

	/* TableWall C */
	TableWall* tableWallC = new TableWall(WALL_C);

	sceneManager->addGraphicObject(tableWallC);
	OBJ_Reader::readFile("testcube", tableWallC);
	editor->loadTransformation(tableWallC);

	/* TableWall D */
	TableWall* tableWallD = new TableWall(WALL_D);

	sceneManager->addGraphicObject(tableWallD);
	OBJ_Reader::readFile("testcube", tableWallD);
	editor->loadTransformation(tableWallD);

	/* Platform Triangle */
	TableWall* platform = new TableWall(PLATFORM);

	sceneManager->addGraphicObject(platform);
	OBJ_Reader::readFile("testcube", platform);
	editor->loadTransformation(platform);

	/* Dragon */
	GraphicObject* dragonHead = new GraphicObject(DRAGON_HEAD);

	sceneManager->addGraphicObject(dragonHead);
	OBJ_Reader::readFile("testcube", dragonHead);
	editor->loadTransformation(dragonHead);

	GraphicObject* dragonLeftArm = new GraphicObject(DRAGON_LEFT_ARM);

	sceneManager->addGraphicObject(dragonLeftArm);
	OBJ_Reader::readFile("testcube", dragonLeftArm);
	editor->loadTransformation(dragonLeftArm);

	GraphicObject* dragonRightArm = new GraphicObject(DRAGON_RIGHT_ARM);

	sceneManager->addGraphicObject(dragonRightArm);
	OBJ_Reader::readFile("testcube", dragonRightArm);
	editor->loadTransformation(dragonRightArm);

	GraphicObject* dragonBody = new GraphicObject(DRAGON_BODY);

	sceneManager->addGraphicObject(dragonBody);
	OBJ_Reader::readFile("testcube", dragonBody);
	editor->loadTransformation(dragonBody);

	GraphicObject* dragonTail = new GraphicObject(DRAGON_TAIL);

	sceneManager->addGraphicObject(dragonTail);
	OBJ_Reader::readFile("testcube", dragonTail);
	editor->loadTransformation(dragonTail);

	GraphicObject* dragonTail2 = new GraphicObject(DRAGON_TAIL_2);

	sceneManager->addGraphicObject(dragonTail2);
	OBJ_Reader::readFile("testcube", dragonTail2);
	editor->loadTransformation(dragonTail2);

	/* Depth 1 nodes */

		/* Table */
		SceneNode* tableNode = new SceneNode(TABLE);
		tableNode->setGraphicObject(table);
		tableNode->setShaderProgram(realWoodShader);

		SceneNode* tableSurfaceNode = new SceneNode(TABLE_SURFACE);
		tableSurfaceNode->setGraphicObject(tableSurface);
		tableSurfaceNode->setShaderProgram(bumpMapShader);

		/* Collision Pins */
		SceneNode* pinANode = new SceneNode(PIN_A);
		pinANode->setGraphicObject(pinA);
		pinANode->setShaderProgram(realWoodShader);

		SceneNode* pinBNode = new SceneNode(PIN_B);
		pinBNode->setGraphicObject(pinB);
		pinBNode->setShaderProgram(realWoodShader);

		SceneNode* pinCNode = new SceneNode(PIN_C);
		pinCNode->setGraphicObject(pinC);
		pinCNode->setShaderProgram(realWoodShader);

		SceneNode* malletNode = new SceneNode(MALLET);
		malletNode->setGraphicObject(mallet);
		malletNode->setShaderProgram(bumpMapShader);

		SceneNode* puckNode = new SceneNode(PUCK);
		puckNode->setGraphicObject(puck);
		puckNode->setShaderProgram(bumpMapShader);

		/* Collision Walls */
		SceneNode* wallANode = new SceneNode(WALL_A);
		wallANode->setGraphicObject(tableWallA);

		SceneNode* wallBNode = new SceneNode(WALL_B);
		wallBNode->setGraphicObject(tableWallB);

		SceneNode* wallCNode = new SceneNode(WALL_C);
		wallCNode->setGraphicObject(tableWallC);

		SceneNode* wallDNode = new SceneNode(WALL_D);
		wallDNode->setGraphicObject(tableWallD);

		/* Collision Platform */
		SceneNode* platformNode = new SceneNode(PLATFORM);
		platformNode->setGraphicObject(platform);

		/* Dragon */
		SceneNode* dragonHeadNode = new SceneNode(DRAGON_HEAD);
		dragonHeadNode->setGraphicObject(dragonHead);

		SceneNode* dragonRightArmNode = new SceneNode(DRAGON_RIGHT_ARM);
		dragonRightArmNode->setGraphicObject(dragonRightArm);

		SceneNode* dragonLeftArmNode = new SceneNode(DRAGON_LEFT_ARM);
		dragonLeftArmNode->setGraphicObject(dragonLeftArm);

		SceneNode* dragonBodyNode = new SceneNode(DRAGON_BODY);
		dragonBodyNode->setGraphicObject(dragonBody);

		SceneNode* dragonTailNode= new SceneNode(DRAGON_TAIL);
		dragonTailNode->setGraphicObject(dragonTail);

		SceneNode* dragonTail2Node= new SceneNode(DRAGON_TAIL_2);
		dragonTail2Node->setGraphicObject(dragonTail2);

		dragonBodyNode->addChildNode(dragonHeadNode);
		dragonBodyNode->addChildNode(dragonRightArmNode);
		dragonBodyNode->addChildNode(dragonLeftArmNode);
		dragonBodyNode->addChildNode(dragonTailNode);
		dragonTailNode->addChildNode(dragonTail2Node);

		sceneManager->addSceneNode(tableNode);
		sceneManager->addSceneNode(tableSurfaceNode);

		sceneManager->addSceneNode(pinANode);
		sceneManager->addSceneNode(pinBNode);
		sceneManager->addSceneNode(pinCNode);

		sceneManager->addSceneNode(malletNode);
		sceneManager->addSceneNode(puckNode);

		sceneManager->addSceneNode(wallANode);
		sceneManager->addSceneNode(wallBNode);
		sceneManager->addSceneNode(wallCNode);
		sceneManager->addSceneNode(wallDNode);

		sceneManager->addSceneNode(dragonBodyNode);

		sceneManager->addSceneNode(platformNode);

	/* Init */
	sceneManager->loadUniforms();

	framebuffer = FrameBuffer::getInstance();
	framebuffer->init(windowWidth,windowHeight);

	postprocessing = new PostProcessing(POST_PROC_NAME,POST_PROC_VERTEX_SHADER_FILE,POST_PROC_FRAGMENT_SHADER_FILE);

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