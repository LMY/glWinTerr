
#ifndef _Y3DGRID_H
#define _Y3DGRID_H

class y3dGrid
{
private:
	void init(int nnodes, float spacing);

	int grid_play; // = 1;

	int gridN;
	float gridS;
	float* grid; // = NULL;
	float* grid_speed; // = NULL;
	float grid_height; //=1;
	float gridCr, gridCg, gridCb; // 0.1, 0.3, 0.7
	int gridType;	// 0:wave, 1:histo(default)

public:
	y3dGrid(int nnodes, float spacing);
	~y3dGrid();

	void clear();
	void random();
	void draw();
	void smooth();
	void map();
	void pause();

	void update();

	void addnoise(float k);
	void random(float k);

	int getGridN() const;
	float getGrid(int y, int x) const;
	void setGrid(int y, int x, float h);

	float getSize() const;

	int getGridType() const;
	void toggleGridType();

	float getHeightAt(float x, float y) const;
};

#endif
