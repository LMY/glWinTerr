
#ifndef _YPHYSICS_H
#define _YPHYSICS_H



class yPhysics
{
private:
	static yPhysics* instance;

	yPhysics();

	
	float dt;
	float g;
	float katt;
	float kattg;

public:
	~yPhysics();

	static void init();
	static void free();
	static yPhysics* getInstance();

	float get_dt() const;
	void set_dt(float new_dt);

	float get_g() const;
	void set_g(float new_g);

	float get_katt() const;
	void set_katt(float new_katt);

	float get_kattg() const;
	void set_kattg(float new_kattg);

	static float random();
};


#endif
