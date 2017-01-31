
#include <Windows.h>
//#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "yPhysics.h"
#include "yCamera.h"
#include "y3dGrid.h"
#include "yLifeforms.h"
#include "yOpenGLApp.h"


// Application parameters
const int nsamples = 1;
const int max_lifeforms = 1000;
const int initial_lifeforms = 200;
const int perspawn_lifeforms = 100;



class myApp : public yOpenGLApp
{
private:
	y3dGrid* y3dgrid;
	yLifeformCollection* lifeforms;
	yCamera* camera;

	int mouse_clickx, mouse_clicky;

public:
	myApp(int argc, char **argv, char* program_title);

	void initialize_scene();
	void free_scene();
	void update_scene();
	void draw_scene();
	void redraw_scene();

	void eventKeyPressed(unsigned char key, int x, int y);
	void eventSpecialKeyPressed(int key, int x, int y);
	void eventProcessMouse(int button, int state, int x, int y);
	void eventProcessMouseActiveMotion(int x, int y);
	void eventProcessMousePassiveMotion(int x, int y);
	void eventProcessMouseEntry(int state);
};



void myApp::eventKeyPressed(unsigned char key, int x, int y)
{
	switch (key) {
		case 81:
		case 27: quit(); break;

		case 'h': y3dgrid->addnoise(0.3f); break;
		case 'y': y3dgrid->pause(); break;
		case 'n': y3dgrid->map(); break;
		case 'r': y3dgrid->random(15); break;
		case 'm': y3dgrid->smooth(); break;
		case 'u': y3dgrid->setGrid(y3dgrid->getGridN()/2, y3dgrid->getGridN()/2, y3dgrid->getGrid(y3dgrid->getGridN()/2, y3dgrid->getGridN()/2)+30); break;
		case 't': y3dgrid->toggleGridType();  break;
		case 'z': y3dgrid->clear(); break;

		case 'p': lifeforms->pause(); break;
		case 's' : lifeforms->spawn(perspawn_lifeforms); break;	
   
			   
//		default:
//			printf ("KP: No action for %d.\n", key);
//			break;
	}
}



void myApp::eventSpecialKeyPressed(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:		camera->obsMove(yCamera::MOVE_LEFT); break;
		case GLUT_KEY_RIGHT:	camera->obsMove(yCamera::MOVE_RIGHT); break;	   
	   	case GLUT_KEY_UP:		camera->obsMove(yCamera::MOVE_UP); break;
		case GLUT_KEY_DOWN:		camera->obsMove(yCamera::MOVE_DOWN); break;
		case GLUT_KEY_HOME:		camera->obsMove(yCamera::MOVE_ABOVE); break;
		case GLUT_KEY_END:		camera->obsMove(yCamera::MOVE_BELOW); break;		   
	   
//		default:
//			printf ("SKP: No action for special %d.\n", key);
//			return;
	}

	camera->apply();
}



void myApp::eventProcessMouse(int button, int state, int x, int y)
{
//	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
			mouse_clickx = x;
			mouse_clicky = y;
		}
}



void myApp::eventProcessMouseActiveMotion(int x, int y)
{
//	printf("processMouseActiveMotion(%i, %i)\n", x-mouse_clickx, y-mouse_clickx);

	camera->incTheta((float)-(x-mouse_clickx)/1000*2);
	camera->incGamma((float)-(y-mouse_clicky)/1000*2);
	camera->apply();

	mouse_clickx = x;
	mouse_clicky = y;
}



void myApp::eventProcessMousePassiveMotion(int x, int y)
{
	//	printf("processMousePassiveMotion(%i, %i)\n", x, y);
}



void myApp::eventProcessMouseEntry(int state)
{
//	if (state == GLUT_LEFT)
//		printf("GLUT_LEFT\n");
//	else
//		printf("GLUT_ENTER\n");
}



myApp::myApp(int argc, char **argv, char* program_title)
: y3dgrid(NULL), lifeforms(NULL), camera(NULL), mouse_clickx(0), mouse_clicky(0)
	, yOpenGLApp(argc, argv, program_title)
{}




void myApp::initialize_scene()
{
	yPhysics::init();

	camera = new yCamera(62, 58, 41);
	camera->setTheta(3.94);
	camera->setGamma(-0.34);
	camera->apply();

	y3dgrid = new y3dGrid(50*nsamples, 3/nsamples);
	y3dgrid->pause();
	y3dgrid->map();
	y3dgrid->addnoise(0.3f);
	//y3dgrid->random(15);
	//y3dgrid->smooth();

	lifeforms = new yLifeformCollection(y3dgrid, max_lifeforms);
	lifeforms->spawn(initial_lifeforms);
	
	
	// Lights
	const float Light0_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
	const float Light0_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
	const float Light0_Position[] = { 2.0, 0.0, 5.0, 1.0 };
	const float Light0_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
	const float Light0_SpotDirection[] = { 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, Light0_Position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  Light0_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  Light0_Diffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0_Specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light0_SpotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	glEnable(GL_LIGHT0);

	const float Light1_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
	const float Light1_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
	const float Light1_Position[] = { -2.0, 0.0, 5.0, 1.0 };
	const float Light1_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
	const float Light1_SpotDirection[] = { -1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, Light1_Position);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light1_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light1_Diffuse); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, Light1_Specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Light1_SpotDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glEnable(GL_LIGHT1);
}

void myApp::free_scene()
{
	delete y3dgrid;
	delete lifeforms;
	delete camera;
}

void myApp::update_scene()
{
	y3dgrid->update();
	lifeforms->update();
	lifeforms->fight();
}

void myApp::draw_scene()
{
	y3dgrid->draw();
	lifeforms->draw();
}

void myApp::redraw_scene()
{
	if (camera)
		camera->apply();
}









int main(int argc, char **argv)
{
	//MessageBox(NULL,L"main()",L"check point!",MB_OK);

	myApp* app = new myApp(argc, argv, "yGLLifeforms");
	yOpenGLApp::setActiveApplication(app);

	app->init();


const GLfloat fogColor[4]= {0.15f, 0.05f, 0.25f, 0.4f};
glFogi(GL_FOG_MODE, GL_LINEAR);        // GL_EXP2, GL_LINEAR, GL_EXP
glFogfv(GL_FOG_COLOR, fogColor);
glFogf(GL_FOG_DENSITY, 0.010f);
glHint(GL_FOG_HINT, GL_NICEST);		//GL_DONT_CARE, GL_FASTEST
glFogf(GL_FOG_START, 10.0f);
glFogf(GL_FOG_END, 0.0f);
//glEnable(GL_FOG);


	app->main();
	delete app;

	return 1;
}
