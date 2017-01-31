


#include <stdlib.h>
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "y3dGrid.h"
#include "yPhysics.h"



y3dGrid::y3dGrid(int nnodes, float spacing)
: grid_play(1), grid(NULL), grid_speed(NULL), grid_height(1), gridCr(0.1f), gridCg(0.3f), gridCb(0.7f), gridType(1)
{
	init(nnodes, spacing);
}




void y3dGrid::init(int nnodes, float spacing)
{
	gridN = nnodes;
	gridS = spacing;

	grid = (float*) malloc(gridN*gridN*sizeof(float));
	grid_speed = (float*) malloc(gridN*gridN*sizeof(float));
	clear();
}

y3dGrid::~y3dGrid()
{
	if (grid) {
		free(grid);
		grid = NULL;
	}
}


int y3dGrid::getGridN() const						{ return gridN; }
float y3dGrid::getGrid(int y, int x) const			{ return grid[gridN*y+x]; }
void y3dGrid::setGrid(int y, int x, float h)		{ grid[gridN*y+x] = h; }

int y3dGrid::getGridType() const					{ return gridType; }
void y3dGrid::toggleGridType()						{ gridType=(gridType==0?1:0); }

float y3dGrid::getSize() const					{ return gridN*gridS; }

void y3dGrid::clear()
{
	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++) {
			grid[gridN*y + x] = 0;
			grid_speed[gridN*y + x] = 0;
		}
}



void y3dGrid::pause()
{
	grid_play = (grid_play == 0 ? 1: 0);
}



void y3dGrid::update()
{
	if (!grid_play)
		return;
	
	float deltaT = (float) yPhysics::getInstance()->get_dt();
	float kAtt = (float) yPhysics::getInstance()->get_katt();

	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++) {
			int index = gridN*y + x;

			if (x!=0&&y!=0 && x!= gridN-1 && y != gridN-1) {
				float f=0;

				for (int py=y-1; py<=y+1; py++)
					for (int px=x-1; px<=x+1; px++)
						f += (grid[gridN*py + px]-grid[index]);
				
				grid_speed[index] += f*deltaT;
				grid_speed[index] -= kAtt*deltaT*grid_speed[index];
				grid[index] += grid_speed[index]*deltaT;
				grid_speed[index] += f*deltaT;
				grid_speed[index] -= kAtt*deltaT*grid_speed[index];				
			}
		}
	
}

void y3dGrid::random(float max_height)
{
	grid_height = max_height;
	
	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++)
			grid[gridN*y + x] = max_height*yPhysics::random();		// (x%3)*0.2+(y%3)*0.2;
}

void y3dGrid::addnoise(float k)
{
	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++)
			grid[gridN*y + x] += 2*k*(yPhysics::random()-0.5f);
}


void y3dGrid::smooth()
{
	float* grid1 = (float*) malloc(gridN*gridN*sizeof(float));

	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++) {
			grid1[gridN*y+x] = 0;

			for (int py=-1; py<=+1; py++)
				for (int px=-1; px<=+1; px++)
					if (px+x >= 0 && px+x < gridN    &&    py+y >= 0 && py+y < gridN)
						grid1[gridN*y+x] += grid[gridN*(y+py)+(x+px)];
					else
						grid1[gridN*y+x] += grid[gridN*y+x];

			grid1[gridN*y+x] /= 9;
		}

	free(grid);
	grid=grid1;
}

void y3dGrid::map()
{
#define	h 10
#define	n -10
#define  dim 10
	float map[dim][dim] = { 	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
								{0, h, h, h, h, h, h, h, h, 0 },
								{0, h, 0, 0, 0, 0, 0, 0, h, 0 },
								{0, h, 0, 0, 0, 0, h, h, h, 0 },
								{0, h, h, 0, 0, 0, h, 0, 0, 0 },
								{0, 0, h, 0, 0, 0, h, h, h, h },
								{0, 0, h, h, 0, 0, 0, 0, h, n },
								{0, 0, 0, h, 0, 0, 0, h, h, n },
								{0, 0, 0, h, h, h, h, h, n, n },
								{0, 0, 0, 0, 0, h, n, n, n, n }};
	int x,y;
	const int rep = gridN/dim;

	for (y=0; y<dim; y++)
		for (x=0; x<dim; x++)
			if (map[y][x] == 0)
				map[y][x] = h;
			else if (map[y][x] == h)
				map[y][x] = 0;

	for (y=0; y<gridN; y++)
		for (x=0; x<gridN; x++)
			grid[gridN*y+x] = map[y/rep][x/rep];
	
}

float y3dGrid::getHeightAt(float x, float y) const
{
	const float maxGrid = gridN*gridS/2;
	
	if (x < -maxGrid || x > maxGrid || y <-maxGrid || y > maxGrid)
		return 99999;
	
	int ix = (int) (x/gridS + gridN/2);
	int iy = (int) (y/gridS + gridN/2);

	return getGrid(iy, ix);
}



void y3dGrid::draw()
{
	const float vert_color = 0.2f;
		
	glBegin(GL_QUADS);
	for (int y=0; y<gridN; y++)
		for (int x=0; x<gridN; x++) {
			float px = -gridS*gridN/2 + x*gridS;
			float py = -gridS*gridN/2 + y*gridS;			

			if (x==gridN/2)
				glColor4f(1, 0, 0, 1);
			else if (y==gridN/2)
				glColor4f(1, 1, 1, 1);
			else
				glColor4f(gridCr, gridCg, gridCb, 1);

			if (gridType == 0) {
				if (x == gridN-1 || y == gridN-1)
					continue;

				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(px, py, grid[gridN*y+ x]);
				glVertex3f(px+gridS, py, grid[gridN*y+ x+1]);
				glVertex3f(px+gridS, py+gridS, grid[gridN*(y+1)+ x+1]);
				glVertex3f(px, py+gridS, grid[gridN*(y+1)+ x]);
			}
			else if (gridType == 1) {
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(px, py, grid[gridN*y+ x]);
				glVertex3f(px+gridS, py, grid[gridN*y+ x]);
				glVertex3f(px+gridS, py+gridS, grid[gridN*y+ x]);
				glVertex3f(px, py+gridS, grid[gridN*y+ x]);

				
				if (x != gridN-1) {
					if (y==gridN/2)
						glColor4f(vert_color, vert_color, vert_color, 1);
					else
						glColor4f(gridCr*vert_color, gridCg*vert_color, gridCb*vert_color, 1);

					glNormal3f(-1.0f, 0.0f, 0.0f);
					glVertex3f(px+gridS, py, grid[gridN*y+ x]);
					glVertex3f(px+gridS, py+gridS, grid[gridN*y+ x]);
					glVertex3f(px+gridS, py+gridS, grid[gridN*y+ (x+1)]);
					glVertex3f(px+gridS, py, grid[gridN*y+ (x+1)]);
				}
				
				if (y != gridN-1) {
					if (x==gridN/2)
						glColor4f(1*vert_color, 0, 0, 1);
					else
						glColor4f(gridCr*vert_color, gridCg*vert_color, gridCb*vert_color, 1);
			
					glNormal3f(0.0f, -1.0f, 0.0f);
					glVertex3f(px, py+gridS, grid[gridN*y+x]);
					glVertex3f(px, py+gridS, grid[gridN*(y+1)+x]);
					glVertex3f(px+gridS, py+gridS, grid[gridN*(y+1)+x]);
					glVertex3f(px+gridS, py+gridS, grid[gridN*y+x]);
				}
			}
		}
	
	glEnd();
}
