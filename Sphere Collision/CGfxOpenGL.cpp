#ifdef _WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#include "CGfxOpenGL.h"

// disable implicit float-double casting
#pragma warning(disable:4305)


extern int PosX;
extern int PosY;
extern int PosZ;
extern int LookX;
extern int LookY;
extern int LookZ;
extern int TransOffset;
extern int Roll;
extern int FOV; 


extern HWND HDialog;
const int NumBalls = 3;
extern Vector3 BArrayPos[];
extern Vector3 BArrayVel[];
extern float BArrayColors[][3];
extern float BallRadius;


Vector3 GetFireDirection(float xAngle, float yAngle) {
	Vector3 result, initialDir;
	Matrix4 Rx, Ry;
	char str[64];

    initialDir.set(0, 0, 1);
	Rx.makeRotationMatrixX(xAngle);
	Ry.makeRotationMatrixY(yAngle);

	result = Multiply(Rx, initialDir);
	result = Multiply(Ry, result);

	// debugging - print out aiming direction
	sprintf(str, "%.2f %.2f %.2f", result.x, result.y, result.z);
	SetWindowText(HDialog, str);


	return result;
	
}

CGfxOpenGL::CGfxOpenGL()
{

	pTerrain = NULL;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{   
    // clear to black background
    //glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearColor(0.5, 0.5, 0.5, 0.0);

    // initialize a rotation angle
    m_angle = 0.0f;

    quadric = gluNewQuadric();



    return true;
}

bool CGfxOpenGL::Shutdown()
{
    if (quadric != NULL) {
        gluDeleteQuadric(quadric);
        quadric = NULL;
    }

	if (pTerrain != NULL) {
        delete pTerrain;
        pTerrain = NULL;
    }
  
    return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
    if (height == 0)                    // don't want a divide by zero
    {
        height = 1;                 
    }

    glViewport(0, 0, width, height);        // reset the viewport to new dimensions
    glMatrixMode(GL_PROJECTION);            // set projection matrix current matrix
    glLoadIdentity();                       // reset projection matrix

    // calculate aspect ratio of window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);

    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix

    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
    m_angle += 0.1f;

}


void xZFloor(double y) {
	int wdQuads = 10;           // number of quad faces across (in x)
	int htQuads = 10;           // number fo quad faces into or out of the screen (in y)
	double dx = 10.0;           // horiz space between vertices
	double dz = 10.0;           // vert space between vertices
	double x, z;             // x, y, z, for each vertex
	int iz, ix;                 // integer counters in x and y directions
	int wdVertices, htVertices; // wd & ht of grid in vertices
	int quadX, quadZ;           // integer coutners for each quad in x and y directions
	int topLeft, topRight, bottomLeft, bottomRight; // 1-based index for each vertex of the quad
    Vector3* terrain;

    // calc number of vertices
	wdVertices = wdQuads + 1;
	htVertices = htQuads + 1;

    terrain = new Vector3[wdVertices * htVertices];

    // calc the x, y , z for each vertex
	for (iz=0; iz < htVertices; iz++) {
		for (ix=0; ix < wdVertices; ix++) {
			x = ix * dx;
			z = iz * -dz;

            terrain[iz * wdVertices + ix].x = x;
            terrain[iz * wdVertices + ix].y = y;
            terrain[iz * wdVertices + ix].z = z;
		}
	}


    // calc the vertex indices for each triangle
    // loop through each quad in the grid
	for (quadZ=0; quadZ < htQuads; quadZ++) {
		for (quadX=0; quadX < wdQuads; quadX++) {
            // calc the indexes of the 4 corners of the quad
			topLeft = quadZ * wdVertices + quadX;
            topRight = topLeft + 1;
			bottomLeft = topLeft + wdVertices;
			bottomRight = bottomLeft + 1;

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glBegin(GL_QUADS);
                glColor3f(1, 1, 1);
                glVertex3f(terrain[bottomLeft].x, terrain[bottomLeft].y, terrain[bottomLeft].z);

                glColor3f(1, 1, 1);
                glVertex3f(terrain[bottomRight].x, terrain[bottomRight].y, terrain[bottomRight].z);

                glColor3f(1, 1, 1);
                glVertex3f(terrain[topRight].x, terrain[topRight].y, terrain[topRight].z);

                glColor3f(1, 1, 1);
                glVertex3f(terrain[topLeft].x, terrain[topLeft].y, terrain[topLeft].z);

            glEnd();
		}
	}

    delete[] terrain;
}


void CGfxOpenGL::Render()
{
    // enable z or depth buffer testing
    glEnable(GL_DEPTH_TEST);

    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

    glMatrixMode(GL_PROJECTION);            // set projection matrix current matrix
    glLoadIdentity();                       // reset projection matrix

    // set up camera perspective projection
    gluPerspective(FOV, float(m_windowWidth) / m_windowHeight, 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix

    // set up camera roll and look at point
    glRotatef(Roll, 0.0f, 0.0f, 1.0f);
	gluLookAt(PosX, PosY, PosZ, LookX, LookY, LookZ, 0, 1, 0);

    glEnable(GL_DEPTH_TEST);


	// render the terrain
	pTerrain->Render();

	// render the balls
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i=0; i < NumBalls; i++) {
		glTranslatef(BArrayPos[i].x, BArrayPos[i].y, BArrayPos[i].z);
		glColor3f(BArrayColors[i][0], BArrayColors[i][1], BArrayColors[i][2]);
		gluSphere(quadric, BallRadius, 20, 20);
		glTranslatef(-BArrayPos[i].x, -BArrayPos[i].y, -BArrayPos[i].z);
	}

}