// Terrain.h
// terrain class for a curved ground
// for use with OpenGL

#ifndef Terrain_H
#define Terrain_H

#include "gmath.h"

#ifndef PI
#define PI 3.14159265f
#endif

class Terrain {
   private:
      int                        maxWidthVertices;    // width in vertices
      int                        maxHeightVertices;   // height in vertices
      int                        widthVertices;    // width in vertices
      int                        heightVertices;   // height in vertices
      int                        widthFaces;       // widht in faces
      int                        heightFaces;      // height in faces
      unsigned long              maxNumVertices;
      unsigned long              numVertices;
      int                        numFaces;
      Vector3                    v0, v1, v2, v3;   // in CW winding order
      Vector3                    dv;               // delta v from 0 to 2
	  Vector3                    *vertices;
	  Vector3                    *normals;
      int                        *faceData;

   public:
      Terrain(
         int                  maxWidthVerticesP,
         int                  maxHeightVerticesP,
         Vector3              *v0P,
         Vector3              *v1P,
         Vector3              *v2P,
         Vector3              *v3P
      );

	  void ReInit(
		   Vector3              *v0P,
		   Vector3              *v1P,
		   Vector3              *v2P,
		   Vector3              *v3P
		);


	  ~Terrain() {
		  if (vertices != NULL) {
			  delete[] vertices;
			  vertices = NULL;
		  }
		  if (normals != NULL) {
			  delete[] normals;
			  normals = NULL;
		  }
		  if (faceData != NULL) {
			  delete[] faceData;
			  faceData = NULL;
		  }
	  };


      void Update(
      );


      void GetTerrainPoint(
         Vector3   *pt,
         double    x,
         double    z
      );

      void GetTerrainPoint(
         Vector3   *pt,
         int         i,          // row (from bottom to top)
         int         j           // column (from left to right)
      );

      void GetTerrainNormal(
         Vector3   *n,
         Vector3   *pt
      );

	  bool CheckBoundaryCollision(Vector3* n, Vector3 pt);

	  Vector3 GetCorrection(Vector3 pt);

	  Vector3 GetDirection(Vector3 p1, Vector3 p2);


      void Render(
      );

};

#endif

