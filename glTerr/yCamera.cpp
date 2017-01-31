

#include <math.h>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "yCamera.h"



yCamera::yCamera()
: obsX(0), obsY(0), obsZ(0), obsTheta(0), obsGamma(0)
{}



yCamera::yCamera(double x, double y, double z)
: obsX(x), obsY(y), obsZ(z), obsTheta(0), obsGamma(0)
{}



yCamera::~yCamera()
{}



double yCamera::getTheta() const		{ return obsTheta; }
double yCamera::getGamma() const		{ return obsGamma; }
void yCamera::setTheta(double th)		{ obsTheta = th; }
void yCamera::setGamma(double th)		{ obsGamma = th; }
void yCamera::incTheta(double dth)		{ obsTheta += dth; }
void yCamera::incGamma(double dth)		{ obsGamma += dth; }



double yCamera::getPos(const int i) const
{
	switch (i) {
	case 0 : return obsX;
	case 1: return obsY;
	case 2: return obsZ;
	default: return -999;
	}
}



void yCamera::obsMove(CAMERA_MOVE_DIRECTION dir)
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



void yCamera::moveTo(double x, double y, double z)
{
	obsX = x;
	obsY = y;
	obsZ = z;
}



void yCamera::apply()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1.0, +1.0, -1.0, +1.0, 1.0, 150.0);
		
	double dx = cos(obsGamma) * cos(obsTheta);
	double dy = cos(obsGamma) * sin(obsTheta);
	double dz = sin(obsGamma);

	double upx = -sin(obsGamma) * cos(obsTheta);
	double upy = -sin(obsGamma) * sin(obsTheta);
	double upz = cos(obsGamma);
	
	gluLookAt(obsX, obsY, obsZ, obsX+dx, obsY+dy, obsZ+dz,  upx, upy, upz);

	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

	//	printf("POS = %.2g\t%.2g\t%.2g\t|\t%.3g\t%.3g\n", obsX, obsY, obsZ, obsTheta, obsGamma);
}
