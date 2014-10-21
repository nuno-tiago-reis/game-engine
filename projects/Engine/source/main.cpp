#include "GL/glew.h"
#include "GL/freeglut.h"

#ifdef MEMORY_LEAK
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "XML_Reader.h"
#include "OBJ_Reader.h"

/* Object */
#include "Object.h"

#include "ParticleSystem.h"

/* Post Processing Effects */
#include "Bloom.h"
#include "MotionBlur.h"

/* Lighting */
#include "SpotLight.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"

/* Shaders */
#include "BlinnPhongShader.h"
#include "MixedTextureShader.h"
#include "BumpMappingShader.h"
#include "WoodShader.h"
#include "SphereMappingShader.h"
#include "CubeMappingShader.h"
#include "FireShader.h"

/* Textures */
#include "Texture.h"
#include "CubeTexture.h"
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

Bloom* bloom;
MotionBlur* motionBlur;

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

	/* Destroy the Post Processing Effects */
	delete bloom;
	delete motionBlur;

	/* Destroy the Scene */
	sceneManager->destroyInstance();
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

	//for every effect

	/* Draw to the Motion Blur Bufer */
	glBindFramebuffer(GL_FRAMEBUFFER, bloom->getFrameBuffer()->getFrameBufferObject());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager->draw();

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	/* Draw to the Motion Blur Bufer */
	//glBindFramebuffer(GL_FRAMEBUFFER, bloom->getFrameBuffer()->getFrameBufferObject());

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//motionBlur->draw();

	//glBindFramebuffer(GL_FRAMEBUFFER,0);

	/* Draw to the Screen */
	bloom->draw();

	glutSwapBuffers();
}

void reshape(int weight, int height) {

	windowWidth = weight;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);

	bloom->reshape(windowWidth,windowHeight);
	motionBlur->reshape(windowWidth,windowHeight);

	sceneManager->reshape(windowWidth,windowHeight);
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
	glDepthRange(0.0f,1.0f);
	glClearDepth(1.0f);

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

	/* Create Bump Map Shader*/
	BumpMappingShader* bumpMappingShader = new BumpMappingShader(BUMP_MAPPING_SHADER);
	bumpMappingShader->createShaderProgram();
	bumpMappingShader->bindAttributes();
	bumpMappingShader->linkShaderProgram();
	bumpMappingShader->bindUniforms();

	sceneManager->addShaderProgram(bumpMappingShader);

	/* Create Sphere Map Shader */
	SphereMappingShader* sphereMappingShader = new SphereMappingShader(SPHERE_MAPPING_SHADER);
	sphereMappingShader->createShaderProgram();
	sphereMappingShader->bindAttributes();
	sphereMappingShader->linkShaderProgram();
	sphereMappingShader->bindUniforms();

	sceneManager->addShaderProgram(sphereMappingShader);

	/* Create Cube Map Shader */
	CubeMappingShader* cubeMappingShader = new CubeMappingShader(CUBE_MAPPING_SHADER);
	cubeMappingShader->createShaderProgram();
	cubeMappingShader->bindAttributes();
	cubeMappingShader->linkShaderProgram();
	cubeMappingShader->bindUniforms();

	sceneManager->addShaderProgram(cubeMappingShader);

	/* Create Real Wood Shader */
	WoodShader* woodShader = new WoodShader(WOOD_SHADER);
	woodShader->createShaderProgram();
	woodShader->bindAttributes();
	woodShader->linkShaderProgram();
	woodShader->bindUniforms();

	sceneManager->addShaderProgram(woodShader);

	/* Create Fire Shader */
	FireShader* fireShader = new FireShader(FIRE_SHADER);
	fireShader->createShaderProgram();
	fireShader->bindAttributes();
	fireShader->linkShaderProgram();
	fireShader->bindUniforms();

	sceneManager->addShaderProgram(fireShader);
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

	//sceneManager->addLight(directionalLight0);

	/* Light Source 1 */
	PositionalLight* positionalLight1 = new PositionalLight(POSITIONAL_LIGHT_1);

	positionalLight1->setIdentifier(LIGHT_SOURCE_1);

	positionalLight1->setPosition(Vector(0.0f, 10.0f, 0.0f, 1.0f));
	positionalLight1->setColor(Vector(1.0f, 1.0f, 1.0f, 1.0f));

	positionalLight1->setAmbientIntensity(0.05f);
	positionalLight1->setDiffuseIntensity(0.55f);
	positionalLight1->setSpecularIntensity(0.55f);

	positionalLight1->setConstantAttenuation(0.5f);
	positionalLight1->setLinearAttenuation(0.000675f);
	positionalLight1->setExponentialAttenuation(0.00025f);

	sceneManager->addLight(positionalLight1);

	/* Light Source 2 */
	SpotLight* spotLight2 = new SpotLight(SPOT_LIGHT_2);

	spotLight2->setIdentifier(LIGHT_SOURCE_2);

	spotLight2->setPosition(Vector(0.0f, 10.0f, 0.0f, 1.0f));
	spotLight2->setDirection(Vector(0.0f, -1.0f, 0.0f, 1.0f));
	spotLight2->setColor(Vector(1.0f, 0.0f, 0.0f, 1.0f));

	spotLight2->setCutOff(cos(25.0f * (GLfloat)DEGREES_TO_RADIANS));

	spotLight2->setAmbientIntensity(0.05f);
	spotLight2->setDiffuseIntensity(0.95f);
	spotLight2->setSpecularIntensity(0.95f);

	spotLight2->setConstantAttenuation(0.5f);
	spotLight2->setLinearAttenuation(0.000675f);
	spotLight2->setExponentialAttenuation(0.00025f);

	sceneManager->addLight(spotLight2);
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

	/* Table Surface */
	Object* tableSurface = new Object(TABLE_SURFACE);

		/* Set the Objects Mesh */
		Mesh* tableSurfaceMesh = new Mesh("Surface", "TableSurface.obj", "TableSurface.mtl");
		tableSurface->setMesh(tableSurfaceMesh);

		/* Set the Objects Transform */
		Transform* tableSurfaceTransform = new Transform(TABLE_SURFACE);
		tableSurfaceTransform->setPosition(Vector(0.0f,-7.5f, 0.0f, 1.0f));
		tableSurfaceTransform->setScale(Vector(10.0f, 1.0f, 10.0f, 1.0f));

		tableSurface->setTransform(tableSurfaceTransform);

		/* Set the Objects Material */
		Material* tableSurfaceMaterial = new Material(TABLE_SURFACE, sceneManager->getShaderProgram(BLINN_PHONG_SHADER));
		tableSurface->setMaterial(tableSurfaceMaterial);

	sceneManager->addObject(tableSurface);

	/* Wood-Texture */
	Object* woodObject = new Object(WOOD_OBJECT);

		/* Set the Objects Mesh */
		Mesh* woodObjectMesh = new Mesh("Teapot", "teapot/teapot2.obj", "teapot/GoldTeapot.mtl");
		woodObject->setMesh(woodObjectMesh);

		/* Set the Objects Transform */
		Transform* woodObjectTransform = new Transform(WOOD_OBJECT);
		woodObjectTransform->setPosition(Vector(0.0f,2.0f,0.0f,1.0f));
		woodObjectTransform->setScale(Vector(0.025f,0.025f,0.025f,1.0f));

		woodObject->setTransform(woodObjectTransform);

		/* Set the Objects Material */
		Material* woodObjectMaterial = new Material(WOOD_OBJECT, sceneManager->getShaderProgram(WOOD_SHADER));

		Texture* woodTexture = new GeneratedTexture("Wood Texture", GL_TEXTURE_3D,  0.5f, 1.0f, 2, 0.32f, NOISE_TEXTURE_UNIFORM);
		woodTexture->loadTexture();
		woodObjectMaterial->addTexture(woodTexture);

		woodObject->setMaterial(woodObjectMaterial);

	sceneManager->addObject(woodObject);

	/* Blinn-Phong */
	Object* blinnPhongObject = new Object(BLINN_PHONG_OBJECT);

		/* Set the Objects Mesh */
		Mesh* blinnPhongObjectMesh = new Mesh("Teapot", "teapot/teapot2.obj", "teapot/SilverTeapot.mtl");
		blinnPhongObject->setMesh(blinnPhongObjectMesh);

		/* Set the Objects Transform */
		Transform* blinnPhongObjectTransform = new Transform(BLINN_PHONG_OBJECT);
		blinnPhongObjectTransform->setPosition(Vector(-10.0f,2.5f,0.0f,1.0f));
		blinnPhongObjectTransform->setRotation(Vector(0.0f,90.0f,0.0f,1.0f));
		blinnPhongObjectTransform->setScale(Vector(0.25f,0.25f,0.25f,1.0f));

		blinnPhongObject->setTransform(blinnPhongObjectTransform);

		/* Set the Objects Material */
		Material* blinnPhongObjectMaterial = new Material(BLINN_PHONG_OBJECT, sceneManager->getShaderProgram(BLINN_PHONG_SHADER));
		blinnPhongObject->setMaterial(blinnPhongObjectMaterial);

	sceneManager->addObject(blinnPhongObject);

	/* Bump Mapping */
	Object* bumpMappingObject = new Object(BUMP_MAPPING_OBJECT);

		/* Set the Objects Mesh */
		Mesh* bumpMappingObjectMesh = new Mesh("Teapot", "teapot/teapot2.obj", "teapot/SilverTeapot.mtl");
		bumpMappingObject->setMesh(bumpMappingObjectMesh);

		/* Set the Objects Transform */
		Transform* bumpMappingObjectTransform = new Transform(BUMP_MAPPING_OBJECT);
		bumpMappingObjectTransform->setPosition(Vector(10.0f,2.5f,0.0f,1.0f));
		bumpMappingObjectTransform->setRotation(Vector(0.0f,90.0f,0.0f,1.0f));
		bumpMappingObjectTransform->setScale(Vector(0.25f,0.25f,0.25f,1.0f));

		bumpMappingObject->setTransform(bumpMappingObjectTransform);

		/* Set the Objects Material */
		Material* bumpMappingObjectMaterial = new Material(BUMP_MAPPING_OBJECT, sceneManager->getShaderProgram(BUMP_MAPPING_SHADER));

		Texture* diffuseTexture = new Texture("Stonefield Diffuse Texture", GL_TEXTURE_2D, DIFFUSE_TEXTURE_UNIFORM, "textures/fieldstone_diffuse.jpg");
		diffuseTexture->loadTexture();
		bumpMappingObjectMaterial->addTexture(diffuseTexture);

		Texture* normalTexture = new Texture("Stonefield Normal Texture", GL_TEXTURE_2D, NORMAL_TEXTURE_UNIFORM, "textures/fieldstone_normal.jpg");
		normalTexture->loadTexture();
		bumpMappingObjectMaterial->addTexture(normalTexture);

		bumpMappingObject->setMaterial(bumpMappingObjectMaterial);

	sceneManager->addObject(bumpMappingObject);

	/* Sphere Environmental Mapping */
	Object* sphereMappingObject = new Object(SPHERE_MAPPING_OBJECT);

		/* Set the Objects Mesh */
		Mesh* sphereMappingObjectMesh = new Mesh("Sphere", "Sphere.obj","Sphere.mtl");
		sphereMappingObject->setMesh(sphereMappingObjectMesh);

		/* Set the Objects Transform */
		Transform* sphereMappingObjectTransform = new Transform(SPHERE_MAPPING_OBJECT);
		sphereMappingObjectTransform->setPosition(Vector(0.0f, 2.5f,-10.0f,1.0f));
		sphereMappingObjectTransform->setScale(Vector(2.5f,2.5f,2.5f,1.0f));

		sphereMappingObject->setTransform(sphereMappingObjectTransform);

		/* Set the Objects Material */
		Material* sphereMappingObjectMaterial = new Material(SPHERE_MAPPING_OBJECT, sceneManager->getShaderProgram(SPHERE_MAPPING_SHADER));

		Texture* sphereMappingTexture = new Texture("House Texture", GL_TEXTURE_2D, SPHERE_MAPPING_UNIFORM, "textures/SphereMap.jpg");
		sphereMappingTexture->loadTexture();
		sphereMappingObjectMaterial->addTexture(sphereMappingTexture);

		sphereMappingObject->setMaterial(sphereMappingObjectMaterial);

	sceneManager->addObject(sphereMappingObject);

	/* Cube Environmental Mapping */
	Object* cubeMappingObject = new Object(CUBE_MAPPING_OBJECT);

		/* Set the Objects Mesh */
		Mesh* cubeMappingObjectMesh = new Mesh("Sphere", "Sphere.obj","Sphere.mtl");
		cubeMappingObject->setMesh(cubeMappingObjectMesh);

		/* Set the Objects Transform */
		Transform* cubeMappingObjectTransform = new Transform(SPHERE_MAPPING_OBJECT);
		cubeMappingObjectTransform->setPosition(Vector(0.0f, 2.5f,10.0f,1.0f));
		cubeMappingObjectTransform->setRotation(Vector(0.0f,180.0f,0.0f,1.0f));
		cubeMappingObjectTransform->setScale(Vector(2.5f,2.5f,2.5f,1.0f));

		cubeMappingObject->setTransform(cubeMappingObjectTransform);

		/* Set the Objects Material */
		Material* cubeMappingObjectMaterial = new Material(CUBE_MAPPING_OBJECT, sceneManager->getShaderProgram(CUBE_MAPPING_SHADER));

		Texture* cubeMappingTexture = new CubeTexture(
			"Beach Texture",
			"textures/beach/posx.jpg","textures/beach/negx.jpg",
			"textures/beach/posy.jpg","textures/beach/negy.jpg",
			"textures/beach/posz.jpg","textures/beach/negz.jpg", 
			SPHERE_MAPPING_UNIFORM);
		cubeMappingTexture->loadTexture();
		cubeMappingObjectMaterial->addTexture(cubeMappingTexture);

		cubeMappingObject->setMaterial(cubeMappingObjectMaterial);

	sceneManager->addObject(cubeMappingObject);

	/* Particle System */
	ParticleSystem* particleSystem = new ParticleSystem(TEST_PARTICLE_SYSTEM, 250, 4.0f);

		/* Set the Objects Mesh */
		Mesh* particleSystemMesh = new Mesh("Particle", "Cube.obj", "Cube.mtl");

		particleSystem->setMesh(particleSystemMesh);

		/* Set the Objects Transform */
		Transform* particleSystemTransform = new Transform(TEST_PARTICLE_SYSTEM);
		particleSystemTransform->setPosition(Vector(0.0f, 2.5f, 0.0f, 1.0f));

		particleSystem->setTransform(particleSystemTransform);

		/* Set the Objects Material */
		Material* particleSystemMaterial = new Material(TEST_PARTICLE_SYSTEM, sceneManager->getShaderProgram(FIRE_SHADER));

		particleSystem->setMaterial(particleSystemMaterial);

		/* Initialize the Particles */
		particleSystem->init();
	
	sceneManager->addObject(particleSystem);

	/* Destroy the Readers */
	OBJ_Reader::destroyInstance();
	XML_Reader::destroyInstance();

	/* Create Scene Graph Nodes */
	SceneNode* tableSurfaceNode = new SceneNode(TABLE_SURFACE);
	tableSurfaceNode->setObject(tableSurface);

	SceneNode* woodObjectNode = new SceneNode(WOOD_OBJECT);
	woodObjectNode->setObject(woodObject);

	SceneNode* blinnPhongObjectNode = new SceneNode(BLINN_PHONG_OBJECT);
	blinnPhongObjectNode->setObject(blinnPhongObject);

	SceneNode* bumpMappingObjectNode = new SceneNode(BUMP_MAPPING_OBJECT);
	bumpMappingObjectNode->setObject(bumpMappingObject);

	SceneNode* sphereEnvironmentalMappingObjectNode = new SceneNode(SPHERE_MAPPING_OBJECT);
	sphereEnvironmentalMappingObjectNode->setObject(sphereMappingObject);

	SceneNode* cubeEnvironmentalMappingObjectNode = new SceneNode(CUBE_MAPPING_OBJECT);
	cubeEnvironmentalMappingObjectNode->setObject(cubeMappingObject);

	SceneNode* particleSystemNode = new SceneNode(TEST_PARTICLE_SYSTEM);
	particleSystemNode->setObject(particleSystem);

	/* Add the Root Nodes to the Scene */
	sceneManager->addSceneNode(tableSurfaceNode);

	sceneManager->addSceneNode(woodObjectNode);
	sceneManager->addSceneNode(blinnPhongObjectNode);
	sceneManager->addSceneNode(bumpMappingObjectNode);
	sceneManager->addSceneNode(sphereEnvironmentalMappingObjectNode);
	sceneManager->addSceneNode(cubeEnvironmentalMappingObjectNode);

	sceneManager->addSceneNode(particleSystemNode);

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

	bloom = new Bloom("Post Processing Effect - Bloom");
	bloom->init(windowWidth, windowHeight);

	motionBlur = new MotionBlur("Post Processing Effect - Motion Blur");
	motionBlur->init(windowWidth, windowHeight);

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