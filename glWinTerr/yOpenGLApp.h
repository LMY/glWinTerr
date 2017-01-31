
#ifndef _YOPENGLAPP_H
#define _YOPENGLAPP_H

#define KEY_SIZE 256

class yOpenGLApp
{
private:
	int Window_ID;
	int Window_Width;
	int Window_Height;

	static yOpenGLApp* app;

	bool keys[KEY_SIZE];

public:
	yOpenGLApp(int argc, char **argv, char* program_title);
	~yOpenGLApp();

	static void setActiveApplication(yOpenGLApp* application);
	static inline yOpenGLApp* getActiveApplication();

	void init();
	void main();
	void quit();

	void eventRender();
	void eventResize(int Width, int Height);


	virtual void initialize_scene() = 0;
	virtual void free_scene() = 0;
	virtual void update_scene() = 0;
	virtual void draw_scene() = 0;
	virtual void redraw_scene() = 0;	// called on resizes, fullscreen_toggle, ...
	
	virtual void eventKeyPressed(unsigned char key, int x, int y) = 0;
	virtual void eventSpecialKeyPressed(int key, int x, int y) = 0;
	virtual void eventProcessMouse(int button, int state, int x, int y) = 0;
	virtual void eventProcessMouseActiveMotion(int x, int y) = 0;
	virtual void eventProcessMousePassiveMotion(int x, int y) = 0;
	virtual void eventProcessMouseEntry(int state) = 0;
};



#endif
