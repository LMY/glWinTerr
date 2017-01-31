
#include "yPhysics.h"
#include <time.h>
#include <stdlib.h>


yPhysics* yPhysics::instance = NULL;


yPhysics::yPhysics()
:dt(0.005f), g(9.81f), katt(0.25f), kattg(0.1f)
{
	srand((unsigned int) time(0));
}



yPhysics::~yPhysics()
{}



void yPhysics::init()
{
	instance = new yPhysics();
}



float yPhysics::random()
{
	return (float)((int)rand())/RAND_MAX;
	//return (float) ((int)rand() % 100000)/100000;
}



void yPhysics::free()
{
	delete instance;
	instance = NULL;
}



yPhysics* yPhysics::getInstance()				{ return instance; }

float yPhysics::get_dt() const					{ return dt; }
void yPhysics::set_dt(float new_dt)			{ dt = new_dt; }
float yPhysics::get_g() const					{ return g; }
void yPhysics::set_g(float new_g)				{ g = new_g; }
float yPhysics::get_katt() const				{ return katt; }
void yPhysics::set_katt(float new_katt)		{ katt = new_katt; }
float yPhysics::get_kattg() const				{ return kattg; }
void yPhysics::set_kattg(float new_kattg)		{ kattg = new_kattg; }
