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
#include "BumpMapShader.h"
#include "RealWoodShader.h"

/* Textures */
#include "Texture.h"
#include "GeneratedTexture.h"

/* Scene Manager */
#include "SceneManager.h"

#define FPS_60	1000/60

#define CAPTION	"AVT 2013"

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

	framebuffer->destroyInstance();
	postProcessing->destroyInstance();

	OBJ_Reader::destroyInstance();
	XML_Reader::destroyInstance();
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
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFrameBufferObject());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager->draw();

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	/* Draw to the Screen */
	postProcessing->draw(framebuffer);

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

	/* Light Source 0 */
	PositionalLight* positionalLight0 = new PositionalLight(POSITIONAL_LIGHT_0);

	positionalLight0->setIdentifier(LIGHT_SOURCE_0);

	positionalLight0->setPosition(Vector(0.0f,-10.0f, 10.0f, 1.0f));
	positionalLight0->setColor(Vector(1.0f, 0.0f, 0.0f, 1.0f));

	positionalLight0->setAmbientIntensity(0.2f);
	positionalLight0->setDiffuseIntensity(0.50f);
	positionalLight0->setSpecularIntensity(0.50f);

	positionalLight0->setConstantAttenuation(1.0f);
	positionalLight0->setLinearAttenuation(0.0f);
	positionalLight0->setExponentialAttenuation(0.0f);

	sceneManager->addLight(positionalLight0);

	/* Light Source 1 */
	PositionalLight* positionalLight1 = new PositionalLight(POSITIONAL_LIGHT_1);

	positionalLight1->setIdentifier(LIGHT_SOURCE_1);

	positionalLight1->setPosition(Vector(0.0f, 10.0f, 10.0f, 1.0f));
	positionalLight1->setColor(Vector(0.0f, 1.0f, 0.0f, 1.0f));

	positionalLight1->setAmbientIntensity(0.2f);
	positionalLight1->setDiffuseIntensity(0.50f);
	positionalLight1->setSpecularIntensity(0.50f);

	positionalLight1->setConstantAttenuation(1.0f);
	positionalLight1->setLinearAttenuation(0.0f);
	positionalLight1->setExponentialAttenuation(0.0f);

	sceneManager->addLight(positionalLight1);

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

	sceneManager->addLight(positionalLight2);

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
	OBJ_Reader* objReader = OBJ_Reader::getInstance();
	XML_Reader* xmlReader = XML_Reader::getInstance();

	xmlReader->openTransformationFile(TRANSFORMATION_FILE);

	/* Table */
	Object* table = new Object(TABLE);
	table->activateWoodTexture(1.0f,2.0f,4,0.16f);
	
	objReader->loadModel(TABLE, table);
	xmlReader->loadTransformation(table);

	sceneManager->addObject(table);

	/* Table Surface */
	Object* tableSurface = new Object(TABLE_SURFACE);
	tableSurface->activateBumpTexture("textures/tampo_diffuse.png","textures/tampo_bump.png");

	objReader->loadModel(TABLE_SURFACE, tableSurface);
	xmlReader->loadTransformation(tableSurface);

	sceneManager->addObject(tableSurface);

	/* Mallet */
	Object* mallet = new Object(MALLET);
	mallet->activateBumpTexture("textures/mallet_diffuse.png","textures/mallet_bump.png");

	objReader->loadModel(MALLET, mallet);	
	xmlReader->loadTransformation(mallet);

	sceneManager->addObject(mallet);

	/* Puck */
	Object* puck = new Object(PUCK);
	puck->activateBumpTexture("textures/puck_diffuse.png","textures/puck_bump.png");

	objReader->loadModel(PUCK, puck);
	xmlReader->loadTransformation(puck);

	sceneManager->addObject(puck);

	/* Pin A */
	Object* pinA = new Object(PIN_A);
	pinA->activateWoodTexture(1.0f,1.0f,4,0.3f);

	objReader->loadModel("testcircle", pinA);	
	xmlReader->loadTransformation(pinA);

	sceneManager->addObject(pinA);

	/* Pin B */
	Object* pinB = new Object(PIN_B);
	pinB->activateWoodTexture(1.0f,1.0f,4,0.3f);

	objReader->loadModel("testcircle", pinB);
	xmlReader->loadTransformation(pinB);

	sceneManager->addObject(pinB);

	/* Pin C */
	Object* pinC = new Object(PIN_C);
	pinC->activateWoodTexture(1.0f,1.0f,4,0.3f);

	objReader->loadModel("testcircle", pinC);
	xmlReader->loadTransformation(pinC);

	sceneManager->addObject(pinC);

	/* Platform */
	Object* platform = new Object(PLATFORM);

	objReader->loadModel("testcube", platform);
	xmlReader->loadTransformation(platform);

	sceneManager->addObject(platform);

	/* Dragon Head */
	JointObject* dragonHead = new JointObject(DRAGON_HEAD);
	dragonHead->setJointPosition(Vector(0.0f,1.25f,0.0f,1.0f));
	dragonHead->setRotation(Vector(15.0f,0.0f,15.0f,1.0f));
	dragonHead->setScale(Vector(0.75f,0.75f,0.50f,1.0f));

	objReader->loadModel("testcube", dragonHead);

	sceneManager->addObject(dragonHead);

	JointObject* dragonNose = new JointObject(DRAGON_NOSE);
	dragonNose->setJointPosition(Vector(0.0f,0.25f,-0.15f,1.0f));
	dragonNose->setRotation(Vector(0.0f,0.0f,0.0f,1.0f));
	dragonNose->setScale(Vector(0.5f,1.0f,0.35f,1.0f));

	objReader->loadModel("testcube", dragonNose);

	sceneManager->addObject(dragonNose);

	JointObject* dragonLeftHorn = new JointObject(DRAGON_LEFT_HORN);
	dragonLeftHorn->setJointPosition(Vector(-0.25f,0.25f,0.25f,1.0f));
	dragonLeftHorn->setRotation(Vector(0.0f,0.0f,0.0f,1.0f));
	dragonLeftHorn->setScale(Vector(0.10f,0.50f,0.10f,1.0f));

	objReader->loadModel("testcube", dragonLeftHorn);

	sceneManager->addObject(dragonLeftHorn);

	JointObject* dragonRightHorn = new JointObject(DRAGON_RIGHT_HORN);
	dragonRightHorn->setJointPosition(Vector(0.25f,0.25f,0.25f,1.0f));
	dragonRightHorn->setRotation(Vector(0.0f,0.0f,0.0f,1.0f));
	dragonRightHorn->setScale(Vector(0.10f,0.50f,0.10f,1.0f));

	objReader->loadModel("testcube", dragonRightHorn);

	sceneManager->addObject(dragonHead);

	/* Dragon Body */
	JointObject* dragonBody = new JointObject(DRAGON_BODY);
	dragonBody->setPosition(Vector(5.0f,0.0f,5.0f,1.0f));
	dragonBody->setRotation(Vector(0.0f,0.0f,0.0f,1.0f));
	dragonBody->setScale(Vector(1.0f,2.0f,0.75,1.0f));

	objReader->loadModel("testcube", dragonBody);

	sceneManager->addObject(dragonBody);

	/* Dragon Left Wing */
	JointObject* dragonLeftWing = new JointObject(DRAGON_LEFT_WING);
	dragonLeftWing->setJointPosition(Vector(-0.5f,0.0f,0.0f,1.0f));
	dragonLeftWing->setRotation(Vector(0.0f,7.5f,0.0f,1.0f));
	dragonLeftWing->setScale(Vector(1.5f,0.75f,0.25f,1.0f));

	objReader->loadModel("testcube", dragonLeftWing);

	sceneManager->addObject(dragonLeftWing);

	/* Dragon Left Wing 2 */
	JointObject* dragonLeftWing2 = new JointObject(DRAGON_LEFT_WING_2);
	dragonLeftWing2->setJointPosition(Vector(-1.25f,0.0f,0.0f,1.0f));
	dragonLeftWing2->setRotation(Vector(0.0f,22.5f,0.0f,1.0f));
	dragonLeftWing2->setScale(Vector(1.75f,2.0f,0.25f,1.0f));

	objReader->loadModel("testcube", dragonLeftWing2);

	sceneManager->addObject(dragonLeftWing2);

	/* Dragon Right Wing */
	JointObject* dragonRightWing = new JointObject(DRAGON_RIGHT_WING);
	dragonRightWing->setJointPosition(Vector(0.5f,0.0f,0.0f,1.0f));
	dragonRightWing->setRotation(Vector(0.0f,-7.5f,0.0f,1.0f));
	dragonRightWing->setScale(Vector(1.5f,0.75f,0.25f,1.0f));

	objReader->loadModel("testcube", dragonRightWing);

	sceneManager->addObject(dragonRightWing);

	/* Dragon Right Wing 2 */
	JointObject* dragonRightWing2 = new JointObject(DRAGON_RIGHT_WING_2);
	dragonRightWing2->setJointPosition(Vector(1.25f,0.0f,0.0f,1.0f));
	dragonRightWing2->setRotation(Vector(0.0f,-22.5f,0.0f,1.0f));
	dragonRightWing2->setScale(Vector(1.75f,2.0f,0.25f,1.0f));

	objReader->loadModel("testcube", dragonRightWing2);

	sceneManager->addObject(dragonRightWing2);

	/* Dragon Tail */
	JointObject* dragonTail = new JointObject(DRAGON_TAIL);
	dragonTail->setJointPosition(Vector(0.0f,-1.50f,0.0f,1.0f));
	dragonTail->setRotation(Vector(7.5f,0.0f,0.0f,1.0f));
	dragonTail->setScale(Vector(0.75f,1.0f,0.50f,1.0f));

	objReader->loadModel("testcube", dragonTail);

	sceneManager->addObject(dragonTail);

	/* Dragon Tail 2 */
	JointObject* dragonTail2 = new JointObject(DRAGON_TAIL_2);
	dragonTail2->setJointPosition(Vector(0.0f,-0.75f,0.0f,1.0f));
	dragonTail2->setRotation(Vector(7.5f,0.0f,0.0f,1.0f));
	dragonTail2->setScale(Vector(0.50f,0.75f,0.35f,1.0f));

	objReader->loadModel("testcube", dragonTail2);

	sceneManager->addObject(dragonTail2);

	/* Dragon Tail 3 */
	JointObject* dragonTail3 = new JointObject(DRAGON_TAIL_3);
	dragonTail3->setJointPosition(Vector(0.0f,-0.75f,0.0f,1.0f));
	dragonTail3->setRotation(Vector(7.5f,0.0f,0.0f,1.0f));
	dragonTail3->setScale(Vector(0.25f,0.75f,0.25f,1.0f));

	objReader->loadModel("testcube", dragonTail3);

	sceneManager->addObject(dragonTail3);

	/* Create Scene Graph Nodes */

	/* Table */
	SceneNode* tableNode = new SceneNode(TABLE);
	tableNode->setObject(table);
	tableNode->setShaderProgram(realWoodShader);

	SceneNode* tableSurfaceNode = new SceneNode(TABLE_SURFACE);
	tableSurfaceNode->setObject(tableSurface);
	tableSurfaceNode->setShaderProgram(bumpMapShader);

	/* Collision Objects */
	SceneNode* pinANode = new SceneNode(PIN_A);
	pinANode->setObject(pinA);
	pinANode->setShaderProgram(realWoodShader);

	SceneNode* pinBNode = new SceneNode(PIN_B);
	pinBNode->setObject(pinB);
	pinBNode->setShaderProgram(realWoodShader);

	SceneNode* pinCNode = new SceneNode(PIN_C);
	pinCNode->setObject(pinC);
	pinCNode->setShaderProgram(realWoodShader);

	SceneNode* malletNode = new SceneNode(MALLET);
	malletNode->setObject(mallet);
	malletNode->setShaderProgram(bumpMapShader);

	SceneNode* puckNode = new SceneNode(PUCK);
	puckNode->setObject(puck);
	puckNode->setShaderProgram(bumpMapShader);

	/* Collision Platform */
	SceneNode* platformNode = new SceneNode(PLATFORM);
	platformNode->setObject(platform);

	/* Dragon */
	SceneNode* dragonHeadNode = new SceneNode(DRAGON_HEAD);
	dragonHeadNode->setObject(dragonHead);

	SceneNode* dragonNoseNode = new SceneNode(DRAGON_NOSE);
	dragonNoseNode->setObject(dragonNose);

	SceneNode* dragonLeftHornNode = new SceneNode(DRAGON_LEFT_HORN);
	dragonLeftHornNode->setObject(dragonLeftHorn);

	SceneNode* dragonRightHornNode = new SceneNode(DRAGON_RIGHT_HORN);
	dragonRightHornNode->setObject(dragonRightHorn);

	SceneNode* dragonBodyNode = new SceneNode(DRAGON_BODY);
	dragonBodyNode->setObject(dragonBody);

	SceneNode* dragonLeftWingNode = new SceneNode(DRAGON_LEFT_WING);
	dragonLeftWingNode->setObject(dragonLeftWing);

	SceneNode* dragonLeftWingNode2 = new SceneNode(DRAGON_LEFT_WING_2);
	dragonLeftWingNode2->setObject(dragonLeftWing2);

	SceneNode* dragonRightWingNode = new SceneNode(DRAGON_RIGHT_WING);
	dragonRightWingNode->setObject(dragonRightWing);

	SceneNode* dragonRightWingNode2 = new SceneNode(DRAGON_RIGHT_WING_2);
	dragonRightWingNode2->setObject(dragonRightWing2);

	SceneNode* dragonTailNode= new SceneNode(DRAGON_TAIL);
	dragonTailNode->setObject(dragonTail);

	SceneNode* dragonTail2Node= new SceneNode(DRAGON_TAIL_2);
	dragonTail2Node->setObject(dragonTail2);

	SceneNode* dragonTail3Node= new SceneNode(DRAGON_TAIL_3);
	dragonTail3Node->setObject(dragonTail3);

	dragonBodyNode->addChildNode(dragonHeadNode);
		dragonHeadNode->addChildNode(dragonNoseNode);
		dragonHeadNode->addChildNode(dragonLeftHornNode);
		dragonHeadNode->addChildNode(dragonRightHornNode);

	dragonBodyNode->addChildNode(dragonRightWingNode);
		dragonRightWingNode->addChildNode(dragonRightWingNode2);

	dragonBodyNode->addChildNode(dragonLeftWingNode);
		dragonLeftWingNode->addChildNode(dragonLeftWingNode2);

	dragonBodyNode->addChildNode(dragonTailNode);
		dragonTailNode->addChildNode(dragonTail2Node);
			dragonTail2Node->addChildNode(dragonTail3Node);

	/* Add the Root Nodes to the Scene */
	sceneManager->addSceneNode(tableNode);
	sceneManager->addSceneNode(tableSurfaceNode);

	sceneManager->addSceneNode(pinANode);
	sceneManager->addSceneNode(pinBNode);
	sceneManager->addSceneNode(pinCNode);

	sceneManager->addSceneNode(malletNode);
	sceneManager->addSceneNode(puckNode);

	sceneManager->addSceneNode(dragonBodyNode);

	sceneManager->addSceneNode(platformNode);

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

	/* Init the SceneManager */
	sceneManager->init();

	/* Init the FrameBuffer */
	framebuffer = FrameBuffer::getInstance();
	framebuffer->init(windowWidth,windowHeight);

	/* Init the PostProcessing Shader */
	postProcessing = PostProcessing::getInstance();
	postProcessing->createShaderProgram();
	postProcessing->bindAttributes();
	postProcessing->linkShaderProgram();
	postProcessing->bindUniforms();

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