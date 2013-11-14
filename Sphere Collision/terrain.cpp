// Terrain.cpp

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "terrain.h"


Terrain::Terrain(
   int                  maxWidthVerticesP,
   int                  maxHeightVerticesP,
   Vector3              *v0P,
   Vector3              *v1P,
   Vector3              *v2P,
   Vector3              *v3P
) {
   int i, j, k;

   maxWidthVertices = maxWidthVerticesP;
   maxHeightVertices = maxHeightVerticesP;
   widthVertices = maxWidthVertices;
   heightVertices = maxHeightVertices;

   numVertices = widthVertices * heightVertices;
   widthFaces = widthVertices - 1;
   heightFaces = heightVertices - 1;
   numFaces = widthFaces * heightFaces * 2; // 2 triangles in each Terrain cell
   v0 = *v0P;
   v1 = *v1P;
   v2 = *v2P;
   v3 = *v3P;
   dv = Subtract(v2, v0);
   dv.x /= float(widthVertices - 1);
   dv.y /= 1;
   dv.z /= float(heightVertices - 1);

   // set up the vertex and normal and texture data
   vertices = new Vector3[numVertices];
   normals = new Vector3[numVertices];

   for (j=0, k=0; j<heightVertices; j++) {
      for (i=0; i<widthVertices; i++, k++) {
         GetTerrainPoint(&(vertices[k]), float(v0.x + i * dv.x), float(v0.z + j * dv.z));
         GetTerrainNormal(&(normals[k]), &(vertices[k]));
         //vertices[k].u = float(i);   
         //vertices[k].v = float(j);   
         //vertices[k].color = D3DCOLOR_ARGB(0,255,255,255);
      }
   }

   // set up the index data
   faceData = new int[numFaces * 3];

   // note opengl uses a CCW winding order
   // vertices 0, 1, 2, 3 are now in the following configuration
   // 2        3
   //  
   //   
   // 0        1
   for (j=0, k=0; j < heightFaces; j++) {
      for (i=0; i < widthFaces; i++) {
         faceData[k++] = j*widthVertices + i;         // vertex 0
         faceData[k++] = j*widthVertices + i+1;       // vertex 1
         faceData[k++] = (j+1)*widthVertices + i+1;   // vertex 3
                    
         faceData[k++] = j*widthVertices + i;         // vertex 0
         faceData[k++] = (j+1)*widthVertices + i+1;   // vertex 3
         faceData[k++] = (j+1)*widthVertices + i;     // vertex 2
      }
   }
}

void Terrain::ReInit(
   Vector3              *v0P,
   Vector3              *v1P,
   Vector3              *v2P,
   Vector3              *v3P
) {
   int i, j, k;

   v0 = *v0P;
   v1 = *v1P;
   v2 = *v2P;
   v3 = *v3P;
   dv = Subtract(v2, v0);
   dv.x /= float(widthVertices - 1);
   dv.y /= 1;
   dv.z /= float(heightVertices - 1);

   for (j=0, k=0; j<heightVertices; j++) {
      for (i=0; i<widthVertices; i++, k++) {
         GetTerrainPoint(&(vertices[k]), float(v0.x + i * dv.x), float(v0.z + j * dv.z));
         GetTerrainNormal(&(normals[k]), &(vertices[k]));
         //vertices[k].u = float(i);   
         //vertices[k].v = float(j);   
         //vertices[k].color = D3DCOLOR_ARGB(0,255,255,255);
      }
   }

}


void Terrain::Update() 
{
    // not used
}

void Terrain::GetTerrainPoint(
   Vector3   *pt,
   double    x,
   double    z
) 
{
   float dy = float((v2.y - v0.y) / (v2.z - v0.z));
   float y0 = float(v0.y + z * dy);
   float xc = float(0.0);
   float zc = float(0.0);
   float r = float(sqrt((x-xc)*(x-xc) + (z-zc)*(z-zc)));
   float y1, y2;

   pt->x = x;

   // interpolate in both z and x directions across the 
   // quad defined by v0, v1, v2, v3
	y1 = v0.y + ((v3.y - v0.y) / (v3.z - v0.z)) * (z - v0.z);
	y2 = v1.y + ((v2.y - v1.y) / (v2.z - v1.z)) * (z - v1.z);
	pt->y = y1 + ((y2 - y1) / (v1.x - v0.x)) * (x - v0.x);

   // use sin or cos to make a half pipe or valley
   //pt->y = float(y0 - 10*cos(x*2.0*PI/80.0));


   // several versions of y value for flat, sloped, sin/cos wavy
   //pt->y = v0.y + ((v2.y - v0.y) / (v2.z - v0.z)) * (z - v0.z);
   //pt->y = v0.y + ((v2.y - v0.y) / (v2.x - v0.x)) * (x - v0.x);
   //pt->y = float(y0 + 0.5*cos(r*2.0*2*PI/85.0) + 1*sin(x*3.0*PI/50.0));
   //pt->y = 0.0f;

   pt->z = z;   
}

void Terrain::GetTerrainPoint
(
   Vector3   *pt,
   int         i,          // row (from bottom to top)
   int         j           // column (from left to right)
) 
{
   *pt = vertices[j * maxHeightVertices * i];   
}

void Terrain::GetTerrainNormal( Vector3   *n, Vector3   *pt) 
{
	Vector3 a, b;
	Vector3 temp;
	float dx, dz;

	// make deltas 1% of each cell
	dx = (v2.x - v0.x) / maxWidthVertices / 100;
	dz = (v2.z - v0.z) / maxHeightVertices / 100;

	GetTerrainPoint(&temp, pt->x,      pt->z);
	GetTerrainPoint(&a, pt->x + dx,      pt->z);
	a = Subtract(a, temp);
	GetTerrainPoint(&b,       pt->x,      pt->z + dz);
	b = Subtract(b, temp);

	*n = CrossProduct(a, b);

	n->normalize();
}

// given a point, this method will return a translation vector
// that will move the point back inside the terrain boundary
// including vertical y direction (above the surface)
Vector3 Terrain::GetCorrection(Vector3 pt) 
{
	Vector3 result;
	Vector3 surfacePt;
	result.set(0, 0, 0);


	 // check all 4 sides of pool table: +x, -x, +z, -z
	 if (pt.x >= v2.x) 
	 {
		result.x = v2.x - pt.x;
	 }
	 if (pt.x <= v0.x)
	 {
		result.x = v0.x - pt.x;
	 }
	 if (pt.z >= v0.z) 
	 {
		result.z = v0.z - pt.z;
	 }
	 if (pt.z <= v2.z) 
	 {
		result.z = v2.z - pt.z;
	 }


	// check vertical y direction
	GetTerrainPoint(&surfacePt, pt.x, pt.z);
	if (pt.y < surfacePt.y) 
	{
		result.y = surfacePt.y - pt.y;
	}

	return result;

}



// checks pt to see if it has collided with the boundaries of this terrain
// if so, returns true and the normalized normal of this boundary
// returns false otherwise

	bool Terrain::CheckBoundaryCollision(Vector3* n, Vector3 pt)
	{
		 // check 4 edges +x, -x, +z, -z
		 if (pt.x >= v2.x)
		 {
			n->set(-1, 0, 0);
			return true;
		 }

		 if (pt.x <= v0.x) 
		 {
			n->set(1, 0, 0);
			return true;
		 }

		 if (pt.z >= v0.z) 
		 {
			 n->set(0, 0, -1);
			return true;
		 }

		 if (pt.z <= v2.z) 
		 {
			n->set(0, 0, 1);
			return true;
		 }

	 return false;
	}




// returns the normalized direction from 2 points (p1, p2) 
// projected on the surface
// in direction from p1 -> p2
Vector3 Terrain::GetDirection(Vector3 p1, Vector3 p2) {
	Vector3 dir;

	GetTerrainPoint(&p1, p1.x, p1.z);
	GetTerrainPoint(&p2, p2.x, p2.z);

	dir = Subtract(p2, p1);
	dir.normalize();

	return dir;
}

void Terrain::Render ()
{
	int i, index;

    glPolygonMode(GL_FRONT, GL_FILL);

    glBegin(GL_TRIANGLES);
	for (i=0; i < numFaces; i++)
	{
		index = faceData[i * 3];

		glColor3f((vertices[index].x - v0.x) / (v2.x - v0.x), 
			      1, 
				  (vertices[index].z - v0.z) /2 / (v2.z - v0.z) );
        glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);

		index = faceData[i * 3 + 1];

		glColor3f(0, 
			      .5,  
				  0);
        glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);

		index = faceData[i * 3 + 2];

		glColor3f((vertices[index].x - v0.x) / (v2.x - v0.x), 
			      1, 
				  (vertices[index].z - v0.z) / (v2.z - v0.z) );
        glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);

	}


    glEnd();


	/*

	// test code to draw the normals
	// green - normal
	// red - up (y) direction
	Vector3 end;

    glBegin(GL_LINES);
	for (i=0; i < numFaces; i++) {
		index = faceData[i * 3];

		glColor3f(0, 1, 0);
        glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);

		end = Add(vertices[index], normals[index]);
		glColor3f(0, 1, 0);
        glVertex3f(end.x, end.y, end.z);

		glColor3f(1, 0, 0);
        glVertex3f(vertices[index].x, vertices[index].y, vertices[index].z);

		glColor3f(1, 0, 0);
        glVertex3f(vertices[index].x, vertices[index].y + 1, vertices[index].z);

	}
    glEnd();
	/* */


}