// gmath.h

#ifndef GMATH_H
#define GMATH_H

#include <math.h>

const double PI          = 3.14159265359;
const double DTOR        = 0.01745329251994;     // degrees to radians
const double RTOD        = 57.29577951308;       // radians to degrees

// function to get the absolute value of an integer
int Abs(int x);

// function to get the sign (+1 or -1) of an integer
int Sign(int x);

int MIN(int a, int b);
int MAX(int a, int b);

float MAX(float a, float b);
    
float MIN(float a, float b, float c);

float MAX(float a, float b, float c);


// a 3D vector with a homogeneous coordinate
// if not used set w = 1
class Vector3 {
public:
    double x, y, z, w;

    // default constructor
    Vector3();

    // set this vector to a new value
    void set(double x, double y, double z, double w = 1.0);

    // set this vector to a new value
    void set(Vector3 v);

    // scale this vector uniformly
    void scale(double f);

    // return squared length of this vector
    double squaredLength();

    // return length of this vector
    double length();

    // normalize this vector
    void normalize();

	// makes a reflection vector of incident about normal
	// normal must be normalized
	void makeReflectionVector(Vector3 incident, Vector3 normal);

};

Vector3 Add(Vector3 a, Vector3 b);

Vector3 Subtract(Vector3 a, Vector3 b);

Vector3 CrossProduct(Vector3 a, Vector3 b);

// return dot product of a & b
double DotProduct(Vector3 a, Vector3 b);

// projection of a onto b
Vector3 Project(Vector3 a, Vector3 b);

// a 4x4 matrix class
class Matrix4 {
public:
    double m[4][4];

    Matrix4();

    Matrix4(double m00, double m01, double m02, double m03, 
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33);

    void makeIdMatrix();

    void makeTranslationMatrix(double dx, double dy, double dz);

    void makeScaleMatrix(double sx, double sy, double sz);

    // makes rotation matrix about X-axis based on a given angle in degrees
    void makeRotationMatrixX(double angle);

    void makeRotationMatrixX(double sinA, double cosA);

    // makes rotation matrix about Y-axis based on a given angle in degrees
    void makeRotationMatrixY(double angle);

    void makeRotationMatrixY(double sinA, double cosA);

    // makes rotation matrix about Z-axis based on a given angle in degrees
    void makeRotationMatrixZ(double angle);

    void makeRotationMatrixZ(double sinA, double cosA);
};

// multiplies matrix m by vector a
Vector3 Multiply(Matrix4 m, Vector3 a);

// multiplies vector a by matrix m
Vector3 Multiply(Vector3 a, Matrix4 m);

// multiply x X y using loops
Matrix4 Multiply(Matrix4 x, Matrix4 y);

#endif