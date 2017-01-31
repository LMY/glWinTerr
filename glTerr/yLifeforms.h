
#ifndef _Y3DOBJECTS_H
#define _Y3DOBJECTS_H

#include "y3dGrid.h"


#define MASS0 7.5


class yGenericLifeform
{
protected:
		y3dGrid* grid;
public:
	yGenericLifeform(y3dGrid* thegrid);

	virtual bool alive() const = 0;

	virtual void spawn() = 0;
	virtual void random() = 0;
	virtual void draw() = 0;
	virtual void pause() = 0;

	virtual void age() = 0;
	virtual void eat(float x) = 0;
	virtual float die() = 0;

	virtual void update() = 0;
};





class yLifeform : public yGenericLifeform
{
private:
	float pos[3];
	float spd[3];

	int lastDir;
	bool play;

	float mass;
	float speed_mod;
	float fight_mod;
	
public:
	bool alive() const;

	void spawn();
	void random();
	void draw();
	void pause();

	void age();
	void eat(float x);
	float die();
	float getMass() const;
	float getRadius() const;
	static void fight(yLifeform* f1, yLifeform* f2);


	void update();

	float getPos(int i) const;
	float getSpd(int i) const;

	void setPos(float posx, float posy, float posz);
	void setSpd(float spdx, float spdy, float spdz);

	void incPos(float posx, float posy, float posz);
	void incSpd(float spdx, float spdy, float spdz);
	
	yLifeform(y3dGrid* thegrid);
	yLifeform(y3dGrid* thegrid, float pos0x, float pos0y, float pos0z);
	~yLifeform();
};





class yLifeformCollection : public yGenericLifeform
{
private:
	int dim;
	int maxdim;
	yGenericLifeform** lifeforms;

public:
	yLifeformCollection(y3dGrid* thegrid, int _maxdim);
	~yLifeformCollection();

	bool alive() const;

	void spawn();
	void random();
	void draw();
	void pause();

	void age();
	void eat(float x);
	float die();

	void update();

	void clean_deads();
	void spawn(int n);
	void fight();
};


#endif
