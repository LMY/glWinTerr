
#ifndef _YCAMERA_H
#define _YCAMERA_H

class yCamera
{
private:
	double obsX, obsY, obsZ;
	double obsTheta, obsGamma;

public:
	enum CAMERA_MOVE_DIRECTION { MOVE_UP = 0, MOVE_DOWN = 1, MOVE_RIGHT = 2, MOVE_LEFT = 3, MOVE_ABOVE = 4, MOVE_BELOW = 5 };


	yCamera();
	yCamera(double x, double y, double z);
	~yCamera();

	void apply();


	double getTheta() const;
	double getGamma() const;
	void setTheta(double th);
	void setGamma(double th);
	void incTheta(double dth);
	void incGamma(double dth);

	void moveTo(double x, double y, double z);
	double getPos(const int i) const;
	void obsMove(CAMERA_MOVE_DIRECTION dir);
};

#endif
