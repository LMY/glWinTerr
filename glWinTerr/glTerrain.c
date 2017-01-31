
#define PROGRAM_TITLE "glONE"

//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glu32.lib")

#include <Windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>   // Always a good idea.
#include <time.h>    // For our FPS stats.
#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
//#include <GL/glut.h> // GLUT support library.

#include "Objects.h"


unsigned int Texture_ID;
int Window_ID;
int Window_Width = 16*60;
int Window_Height = 9*60;


float Light0_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
float Light0_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
float Light0_Position[] = { 2.0, 0.0, 5.0, 1.0 };
float Light0_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
float Light0_SpotDirection[] = { 1.0, 0.0, 0.0 };

float Light1_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
float Light1_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
float Light1_Position[] = { -2.0, 0.0, 5.0, 1.0 };
float Light1_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
float Light1_SpotDirection[] = { -1.0, 0.0, 0.0 };


y3dGrid* y3dgrid = NULL;
Flyby* flyby = NULL;


#define FRAME_RATE_SAMPLES 50
int FrameCount=0;
float FrameRate=0;

static void ourDoFPS() 
{
   static clock_t last=0;
   clock_t now;
   float delta;

   if (++FrameCount >= FRAME_RATE_SAMPLES) {
      now  = clock();
      delta= (now - last) / (float) CLOCKS_PER_SEC;
      last = now;

      FrameRate = FRAME_RATE_SAMPLES / delta;
      FrameCount = 0;
   }
}



double obsX=62, obsY=58, obsZ=41.0;
double obsTheta=3.94, obsGamma=-0.34;

void apply_projection_matrix()
{
	const double distance = 1;
	double dx, dy, dz;
	double upx, upy, upz;	
	glMatrixMode(GL_PROJECTION);        // Set projection parameters.
	glLoadIdentity();

	//glFrustum(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, 5, 100);	
	glFrustum(-1.0,1.0,-1.0,1.0,1.0,150.0);
//	glOrtho(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, 0.5, 100);
//	glOrtho(-1.0,1.0,-1.0,1.0,1.0,50.0);
	
	dx = distance * cos(obsGamma) * cos(obsTheta);
	dy = distance * cos(obsGamma) * sin(obsTheta);
	dz = distance * sin(obsGamma);

	upx = -sin(obsGamma) * cos(obsTheta);
	upy = -sin(obsGamma) * sin(obsTheta);
	upz = cos(obsGamma);
	
	gluLookAt(obsX, obsY, obsZ, obsX+dx, obsY+dy, obsZ+dz,  upx, upy, upz);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

//	printf("POS = %.2g\t%.2g\t%.2g\t|\t%.3g\t%.3g\n", obsX, obsY, obsZ, obsTheta, obsGamma);
}

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_RIGHT 2
#define MOVE_LEFT 3
#define MOVE_ABOVE 4
#define MOVE_BELOW 5

void obsMove(int dir)
{
	const double mov_speed = 1;

	if (dir == MOVE_UP) {
		   obsX  += mov_speed* cos(obsGamma) * cos(obsTheta);
		   obsY  += mov_speed* cos(obsGamma) * sin(obsTheta);
		   obsZ  += mov_speed* sin(obsGamma);
	}
	else if (dir == MOVE_DOWN) {
		   obsX  -= mov_speed* cos(obsGamma) * cos(obsTheta);
		   obsY  -= mov_speed* cos(obsGamma) * sin(obsTheta);
		   obsZ  -= mov_speed* sin(obsGamma);
	}
	else if (dir == MOVE_RIGHT) {
		   obsX  -= mov_speed* cos(obsGamma) * -sin(obsTheta);
		   obsY  -= mov_speed* cos(obsGamma) * cos(obsTheta);
		   obsZ  -= mov_speed* sin(obsGamma);
	}
	else if (dir == MOVE_LEFT) {
		   obsX  += mov_speed* cos(obsGamma) * -sin(obsTheta);
		   obsY  += mov_speed* cos(obsGamma) * cos(obsTheta);
		   obsZ  += mov_speed* sin(obsGamma);
	}
	else if (dir == MOVE_ABOVE) {
		   obsX  += mov_speed* -sin(obsGamma) * cos(obsTheta);
		   obsY  += mov_speed* -sin(obsGamma) * sin(obsTheta);
		   obsZ  += mov_speed* cos(obsGamma);
	}
	else if (dir == MOVE_BELOW) {
		   obsX  -= mov_speed* -sin(obsGamma) * cos(obsTheta);
		   obsY  -= mov_speed* -sin(obsGamma) * sin(obsTheta);
		   obsZ  -= mov_speed* cos(obsGamma);
	}
}





void cbRenderScene(void)
{
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   // Clear the color and depth buffers.

	
	// Draw
	y3dgrid->draw();
	flyby->draw();

	y3dgrid->update();
	flyby->update();
	
	glutSwapBuffers();
	ourDoFPS();
}



void cbKeyPressed(unsigned char key, int x, int y)
{
   switch (key) {
      case 81: case 27: // Q (Escape) - We're outta here.
      glutDestroyWindow(Window_ID);
      exit(1);
      break;
	   //	case GLUT_KEY_DELETE:
		case 127: obsGamma  -= 0.1;		apply_projection_matrix();break;

	   case 'h': y3dgrid->addnoise(0.3); break;
	   case 'y': y3dgrid->pause(); break;
		case 'n': y3dgrid->map(); break;
   	   case 'r': y3dgrid->random(15); break;
   	   case 'm': y3dgrid->smooth(); break;
	   case 'u':
			y3dgrid->setGrid(y3dgrid->getGridN()/2, y3dgrid->getGridN()/2, y3dgrid->getGrid(y3dgrid->getGridN()/2, y3dgrid->getGridN()/2)+30);
//			grid[gridN*(gridN/2)+gridN/2] += 30;
		   
		   break;
		case 't': y3dgrid->toggleGridType();  break;
		case 'z': y3dgrid->clear(); break;

		case 'p':flyby->pause(); break;
		   
		   
	case 'w' : obsMove(MOVE_UP); break;
	case 's' : obsMove(MOVE_DOWN); break;	
	case 'a' : obsMove(MOVE_LEFT); break;
	case 'd' : obsMove(MOVE_RIGHT); break;		   
			   
   default:
      printf ("KP: No action for %d.\n", key);
      break;
    }
}




void cbSpecialKeyPressed(int key, int x, int y)
{
	const double mov_anglspeed = 0.2;
	
   switch (key) {
	case GLUT_KEY_PAGE_UP: obsTheta  += mov_anglspeed;	break;
	case GLUT_KEY_PAGE_DOWN: obsTheta  -= mov_anglspeed;	break;

	case GLUT_KEY_INSERT: obsGamma  += mov_anglspeed;	break;


	case GLUT_KEY_LEFT:
			obsMove(MOVE_LEFT);
		   break;
	case GLUT_KEY_RIGHT:
			obsMove(MOVE_RIGHT);
		   break;	   
	   
	case GLUT_KEY_UP:
			obsMove(MOVE_UP);
		   break;
		   
	case GLUT_KEY_DOWN:
			obsMove(MOVE_DOWN);
		   break;


	case GLUT_KEY_HOME:
			obsMove(MOVE_ABOVE);		   
		   break;
		   
	case GLUT_KEY_END:
			obsMove(MOVE_BELOW);
		   break;		   
	   
   default:
      printf ("SKP: No action for special %d.\n", key);
      break;
    }

	apply_projection_matrix();
}




void cbResizeScene(int Width, int Height)
{
	if (Height <= 0)
		Height = 1;

	glViewport(0, 0, Width, Height);
	apply_projection_matrix();
	
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45.0f, (GLfloat) Width/(GLfloat)Height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);

	Window_Width  = Width;
	Window_Height = Height;
}



void ourInit(int Width, int Height) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Color to clear color buffer to.
	glClearDepth(1.0);						// Depth to clear depth buffer to; type of test.
	glDepthFunc(GL_LESS); 
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.

	cbResizeScene(Width, Height);			// Load up the correct perspective matrix; using a callback directly.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set up a light, turn it on.
	
	glLightfv(GL_LIGHT0, GL_POSITION, Light0_Position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  Light0_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  Light0_Diffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0_Specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light0_SpotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_POSITION, Light1_Position);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light1_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light1_Diffuse); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, Light1_Specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Light1_SpotDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
	glEnable(GL_LIGHT1);

	
	// A handy trick -- have surface material mirror the color.
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

int mouse_clickx=0, mouse_clicky=0;
void processMouse(int button, int state, int x, int y)
{
//	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
			mouse_clickx = x;
			mouse_clicky = y;
		}
}
void processMouseActiveMotion(int x, int y)
{
//	printf("processMouseActiveMotion(%i, %i)\n", x-mouse_clickx, y-mouse_clickx);

	obsTheta -= (double)(x-mouse_clickx)/1000*2;
	obsGamma -= (double)(y-mouse_clicky)/1000*2;
	apply_projection_matrix();

	mouse_clickx = x;
	mouse_clicky = y;
}

void processMousePassiveMotion(int x, int y) {
	//	printf("processMousePassiveMotion(%i, %i)\n", x, y);
}
void processMouseEntry(int state) {
	/*
	if (state == GLUT_LEFT)
		printf("GLUT_LEFT\n");
	else
		printf("GLUT_ENTER\n");
	*/
}

 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);		   // To see OpenGL drawing, take out the GLUT_DOUBLE request.
	glutInitWindowSize(Window_Width, Window_Height);

	Window_ID = glutCreateWindow(PROGRAM_TITLE); 

	glutDisplayFunc(&cbRenderScene);
	glutIdleFunc(&cbRenderScene); 
	glutReshapeFunc(&cbResizeScene);

	glutKeyboardFunc(&cbKeyPressed);
	glutSpecialFunc(&cbSpecialKeyPressed);

	ourInit(Window_Width, Window_Height); 
 
	//adding here the mouse processing callbacks
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutPassiveMotionFunc(processMousePassiveMotion);
	glutEntryFunc(processMouseEntry);
	
	y3dgrid = new y3dGrid(100, 1.5);
	y3dgrid->random(15);
	y3dgrid->smooth();

	flyby = new Flyby(y3dgrid, 0, 0, 12);
	
	apply_projection_matrix();
	glutMainLoop();

	return 1;
}


