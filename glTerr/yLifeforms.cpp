
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>   // OpenGL itself.

#include "yPhysics.h"
#include "yLifeforms.h"



void yLifeform::spawn()
{
//	pos[0] = pos[1] = pos[2] = spd[0] = spd[1] = spd[2] = 0;
	spd[0] = spd[1] = spd[2] = 0;
	pos[0] = grid->getSize() * (yPhysics::random()-0.5f);
	pos[1] = grid->getSize() * (yPhysics::random()-0.5f);
	pos[2] = grid->getHeightAt(pos[0], pos[1]);


	mass=MASS0;
	speed_mod = yPhysics::random();
	fight_mod = yPhysics::random();
}


void yLifeform::random()
{
	for (int i=0; i<3; i++) {
		pos[i] = yPhysics::random();
		spd[i] = yPhysics::random();
	}
}

void yLifeform::draw()
{
	glBegin(GL_QUADS);

	glColor4f(1-fight_mod, fight_mod, 0, 1);
	glNormal3f(0, 1, 0);
	float k=0.5f*mass/10;

	glVertex3f(pos[0] - k, pos[1] - k, pos[2] + 0.1f);
	glVertex3f(pos[0] + k, pos[1] - k, pos[2] + 0.1f);
	glVertex3f(pos[0] + k, pos[1] + k, pos[2] + 0.1f);
	glVertex3f(pos[0] - k, pos[1] + k, pos[2] + 0.1f);

	glColor4f(1-speed_mod, speed_mod, 0, 1);
	glNormal3f(0, 1, 0);
	k /= 2;
	glVertex3f(pos[0] - k, pos[1] - k, pos[2] + 0.2f);
	glVertex3f(pos[0] + k, pos[1] - k, pos[2] + 0.2f);
	glVertex3f(pos[0] + k, pos[1] + k, pos[2] + 0.2f);
	glVertex3f(pos[0] - k, pos[1] + k, pos[2] + 0.2f);

	glColor4f(1, 1, 0, 1);
	glNormal3f(0, 1, 0);
	k /= 2;
	glVertex3f(pos[0] - k, pos[1] - k, pos[2] + 0.3f);
	glVertex3f(pos[0] + k, pos[1] - k, pos[2] + 0.3f);
	glVertex3f(pos[0] + k, pos[1] + k, pos[2] + 0.3f);
	glVertex3f(pos[0] - k, pos[1] + k, pos[2] + 0.3f);

	glEnd();
}

void yLifeform::pause()						{ play = (play?0:1); }

void yLifeform::eat(float x)
{
	mass += x;
	if (mass < 0) mass = 0;
}

float yLifeform::die()
{
	float m = mass;
	mass = 0;
//	reborn();
	return m * 0.5f;
}

float yLifeform::getMass() const			{ return mass; }
float yLifeform::getRadius() const			{ return 0.05f*mass; }
bool yLifeform::alive() const				{ return (mass>0); }

void yLifeform::age()
{
	mass -= 0.015f*(2.5f-1.5f*speed_mod);		// y=(1-n)speed_mod+n.... speed_mod makes age-ing n times faster at speed_mod=0
}


void yLifeform::fight(yLifeform* f1, yLifeform* f2)
{
	if (!f1->alive() || !f2->alive())
		return;

	const float fight_range = pow(f1->getRadius() + f2->getRadius(), 2);

	int d2=0;
	for (int i=0; i<3; i++)
		d2 += pow(f1->pos[i] - f2->pos[i], 2);

	if (d2 > fight_range)
		return;


	float m1=f1->getMass()*(1+2*f1->fight_mod);
	float m2=f2->getMass()*(1+2*f2->fight_mod);

	if (yPhysics::random()*(m1+m2) < m1)
		f1->eat(f2->die());
	else
		f2->eat(f1->die());
}



#define DELTA_HEIGHT 0.6
void yLifeform::update()
{
	if (!play)
		return;

	age();
	if (mass <= 0) {
		die();
		return;
	}

	int dir = 0;

	int choose = (rand() % 8);
	
	if (choose == 0)
		dir = rand() % 4;
	else if (choose == 1) {
		if (lastDir == 0)
			dir = 2;
		else if (lastDir == 2)
			dir = 1;
		else if (lastDir == 1)
			dir = 3;
		else if (lastDir == 3)
			dir = 0;
	}
	else if (choose == 2) {
		if (lastDir == 0)
			dir = 3;
		else if (lastDir == 3)
			dir = 1;
		else if (lastDir == 1)
			dir = 2;
		else if (lastDir == 2)
			dir = 0;
	}
	else
		dir = lastDir;

	lastDir = dir;
	if (rand() % 4 == 0)
		dir = lastDir;

	float newspd[3];
	float newpos[3];

	float deltaT = yPhysics::getInstance()->get_dt();
	float kAtt = yPhysics::getInstance()->get_katt();
	float kAttg = yPhysics::getInstance()->get_kattg();

	const float Ftrac = 9*(1+2*speed_mod);
	float f[3];
	f[0] = 0;
	f[1] = 0;
	f[2] = 0;

	if (dir==0) f[0] += +Ftrac;
	else if (dir==1) f[0] += -Ftrac;
	else if (dir==2) f[1] += +Ftrac;
	else if (dir==3) f[1] += -Ftrac;

	if (pos[2] < grid->getHeightAt(pos[0], pos[1]))
		pos[2] = grid->getHeightAt(pos[0], pos[1]);
	else
		f[2] = -yPhysics::getInstance()->get_g();


	for (int index=0; index<3; index++) {
		newspd[index] = spd[index] + 10/mass*(1+speed_mod)*f[index]*deltaT;
		newspd[index] -= deltaT*newspd[index]*kAtt;
		newpos[index] = pos[index] + newspd[index]*deltaT;
		newspd[index] = spd[index] + 10/mass*(1+speed_mod)*f[index]*deltaT;
		newspd[index] -= deltaT*newspd[index]*kAtt;
	}

	if (grid->getHeightAt(newpos[0], newpos[1]) >= grid->getHeightAt(pos[0], pos[1]) + DELTA_HEIGHT) {
		spd[0] = spd[1] = 0;
	}
	else {
		if (newpos[2] < grid->getHeightAt(newpos[0], newpos[1]))
			newpos[2] = grid->getHeightAt(newpos[0], newpos[1]);
		
		pos[0] = newpos[0];
		pos[1] = newpos[1];
		pos[2] = newpos[2];
		spd[0] = newspd[0];
		spd[1] = newspd[1];
		spd[2] = newspd[2];
	}
}



float yLifeform::getPos(int i) const		{ return pos[i]; }
float yLifeform::getSpd(int i) const		{ return spd[i]; }

void yLifeform::setPos(float posx, float posy, float posz)	{ pos[0] = posx; pos[1] = posy; pos[2] = posz; }
void yLifeform::setSpd(float spdx, float spdy, float spdz)	{ spd[0] = spdx; spd[1] = spdy; spd[2] = spdz; }

void yLifeform::incPos(float posx, float posy, float posz)	{ pos[0] += posx; pos[1] += posy; pos[2] += posz; }
void yLifeform::incSpd(float spdx, float spdy, float spdz)	{ spd[0] += spdx; spd[1] += spdy; spd[2] += spdz; }


yLifeform::yLifeform(y3dGrid* thegrid)
: yGenericLifeform(thegrid), play(1), lastDir(0), mass(MASS0)
{
	setSpd(0, 0, 0);

	speed_mod = yPhysics::random();
	fight_mod = yPhysics::random();
}

yLifeform::yLifeform(y3dGrid* thegrid, float pos0x, float pos0y, float pos0z)
: yGenericLifeform(thegrid), play(1), lastDir(0), mass(MASS0)
{
	setPos(pos0x, pos0y, pos0z);
	setSpd(0, 0, 0);

	speed_mod = yPhysics::random();
	fight_mod = yPhysics::random();
}


yLifeform::~yLifeform()
{}


yGenericLifeform::yGenericLifeform(y3dGrid* thegrid)
: grid(thegrid)
{}

	


yLifeformCollection::yLifeformCollection(y3dGrid* thegrid, int _maxdim)
: yGenericLifeform(thegrid), dim(0), maxdim(_maxdim)
{
	lifeforms = new yGenericLifeform*[maxdim];
	for (int i=0; i<maxdim; i++)
		lifeforms[i] = NULL;
}

yLifeformCollection::~yLifeformCollection()
{
	for (int i=0; i<dim; i++)
		delete lifeforms[i];

	delete[] lifeforms;
}

bool yLifeformCollection::alive() const
{
	for (int i=0; i<dim; i++)
		if (lifeforms[i]->alive())
			return true;

	return false;
}

void yLifeformCollection::spawn()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->spawn();
}

void yLifeformCollection::random()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->random();
}

void yLifeformCollection::draw()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->draw();
}

void yLifeformCollection::pause()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->pause();
}

void yLifeformCollection::age()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->age();
}

void yLifeformCollection::eat(float x)
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->eat(x/dim);
}

float yLifeformCollection::die()
{
	float s=0;

	for (int i=0; i<dim; i++)
		s += lifeforms[i]->die();

	return s;
}

void yLifeformCollection::update()
{
	for (int i=0; i<dim; i++)
		lifeforms[i]->update();
	
	clean_deads();
}

void yLifeformCollection::fight()
{
	for (int i=0; i<dim; i++)
		for (int k=i+1; k<dim; k++)
			yLifeform::fight(static_cast<yLifeform*>(lifeforms[i]), static_cast<yLifeform*>(lifeforms[k]));
}

void yLifeformCollection::clean_deads()
{
	int nexti=0;

	for (int i=0; i<dim; i++)
		if (lifeforms[i]->alive())
			lifeforms[nexti++] = lifeforms[i];
		else
			delete lifeforms[i];

	for (int i=nexti; i<dim; i++)
		lifeforms[i] = NULL;

	dim = nexti;
}

void yLifeformCollection::spawn(int n)
{
	int newdim = dim+n;
	if (newdim > maxdim) newdim = maxdim;

	for (int i=dim; i<maxdim; i++) {
		lifeforms[i] = new yLifeform(grid);
		lifeforms[i]->spawn();
	}

	dim = newdim;
}
