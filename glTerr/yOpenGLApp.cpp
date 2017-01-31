
#include "yOpenGLApp.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>



// Global variable: application
class yOpenGLApp;
yOpenGLApp* app = NULL;

void cbRender()												{ app->eventRender(); }
void cbResize(int w, int h)									{ app->eventResize(w, h); }
void cbKeyPressed(unsigned char key, int x, int y)			{ app->eventKeyPressed(key, x, y); }
void cbSpecialKeyPressed(int key, int x, int y)				{ app->eventSpecialKeyPressed(key, x, y); }
void cbProcessMouse(int button, int state, int x, int y)	{ app->eventProcessMouse(button, state, x, y); }
void cbProcessMouseActiveMotion(int x, int y)				{ app->eventProcessMouseActiveMotion(x, y); }
void cbProcessMousePassiveMotion(int x, int y)				{ app->eventProcessMousePassiveMotion(x, y); }
void cbProcessMouseEntry(int state)							{ app->eventProcessMouseEntry(state); }



void yOpenGLApp::eventRender()
{
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_scene();
	update_scene();
}



void yOpenGLApp::eventResize(int Width, int Height)
{
	if (Height <= 0)
		Height = 1;

	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_MODELVIEW);

	Window_Width  = Width;
	Window_Height = Height;
}



yOpenGLApp::yOpenGLApp(int argc, char **argv, char* program_title)
: 	Window_Width(16*60), Window_Height(9*60)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(Window_Width, Window_Height);

	Window_ID = glutCreateWindow(program_title); 

	glutDisplayFunc(cbRender);
	glutIdleFunc(cbRender); 
	glutReshapeFunc(cbResize);

	glutKeyboardFunc(cbKeyPressed);
	glutSpecialFunc(cbSpecialKeyPressed);
	glutMouseFunc(cbProcessMouse);
	glutMotionFunc(cbProcessMouseActiveMotion);
//	glutPassiveMotionFunc(cbProcessMousePassiveMotion);
//	glutEntryFunc(cbProcessMouseEntry);
}



yOpenGLApp::~yOpenGLApp()
{}



void yOpenGLApp::init() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS); 
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	eventResize(Window_Width, Window_Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// have surface material mirror the color.
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	initialize_scene();
}



void yOpenGLApp::main()
{
	glutMainLoop();
}



void yOpenGLApp::quit()
{
	glutDestroyWindow(Window_ID);
	free_scene();
	exit(1);
}
