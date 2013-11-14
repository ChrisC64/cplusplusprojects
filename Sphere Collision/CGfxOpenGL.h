#ifndef CGFXOPENGL_H
#define CGFXOPENGL_H

#include <math.h>

#include "terrain.h"
#include "gmath.h"


class CGfxOpenGL
{
public:
	int m_windowWidth;
	int m_windowHeight;

	float m_angle;
    GLUquadricObj* quadric;

	Terrain* pTerrain;

public:
	CGfxOpenGL();
	virtual ~CGfxOpenGL();

	bool Init();
	bool Shutdown();

	void SetupProjection(int width, int height);

	void Prepare(float dt);
	void Render();
};


#endif