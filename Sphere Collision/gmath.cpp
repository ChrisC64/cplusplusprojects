// gmath.cpp

#include "gmath.h"

// function to get the absolute value of an integer
int Abs(int x) {
    if (x < 0)  return -x;
    else        return x;
}

// function to get the sign (+1 or -1) of an integer
int Sign(int x) {
    if (x < 0)  return -1;
    else        return 1;
}

int MIN(int a, int b) {
    if (a < b) return a;
    else return b;
}
int MAX(int a, int b) {
    if (a > b) return a;
    else return b;
}

float MIN(float a, float b) {
    if (a > b) return a;
    else return b;
}

float MAX(float a, float b) {
    if (a > b) return a;
    else return b;
}
    
float MIN(float a, float b, float c) {
    return MIN(MIN(a, b), c);
}

float MAX(float a, float b, float c) {
    return MAX(MAX(a, b), c);
}


// a 3D vector with a homogeneous coordinate
// if not used set w = 1
Vector3::Vector3() {
        x = y = z = 0.0;
        w = 1.0;
    }

    // set this vector to a new value
    void Vector3::set(double x, double y, double z, double w) {
        this->x = x; 
        this->y = y; 
        this->z = z;   
        this->w = w;
    }

    // set this vector to a new value
    void Vector3::set(Vector3 v) {
        this->x = v.x; 
        this->y = v.y; 
        this->z = v.z;   
        this->w = v.w;
    }

    // scale this vector uniformly
    void Vector3::scale(double f) {
        this->x *= f; 
        this->y *= f; 
        this->z *= f;   
    }

    // return squared length of this vector
    double Vector3::squaredLength() {
        return x * x + y * y + z * z;
    }

    // return length of this vector
    double Vector3::length() {
        return sqrt(x * x + y * y + z * z);
    }

    // normalize this vector
    void Vector3::normalize() {
        double len = length();
		if (len < 0.000001) {
			// just reutrn since length is too small to divide
			return;
		}
        scale(1/len);
    }

	// makes a reflection vector of incident about normal
	// normal must be normalized
	void Vector3::makeReflectionVector(Vector3 incident, Vector3 normal) {
	   this->set(normal);
	   this->scale(-2 * DotProduct(incident, normal));
	   *this = Add(incident, *this);
	}


Vector3 Add(Vector3 a, Vector3 b) {
    // addition - returns result = a + b
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 Subtract(Vector3 a, Vector3 b){
    // subtraction - returns result = a - b
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vector3 CrossProduct(Vector3 a, Vector3 b) {
    // cross product - returns result = a X b
    Vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

// return dot product of a & b
double DotProduct(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// projection of a onto b
Vector3 Project(Vector3 a, Vector3 b) {
	// return the projection of vector a onto b
	Vector3 result;
	double len2 = b.squaredLength();

	result.set(b);
	result.scale(DotProduct(b, a) / len2);

	return result;


}



// a 4x4 matrix class
Matrix4::Matrix4(){ 
        // default constructor set m = I
        m[0][0] = 1.0;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = 0.0;
        m[1][0] = 0.0;  m[1][1] = 1.0;  m[1][2] = 0.0;  m[1][3] = 0.0;
        m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] = 1.0;  m[2][3] = 0.0;
        m[3][0] = 0.0;  m[3][1] = 0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;
    }

    Matrix4::Matrix4(double m00, double m01, double m02, double m03, 
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33) {
        m[0][0] = m00;  m[0][1] = m01;  m02 = m02;  m03 = m03;
        m[1][0] = m10;  m[1][1] = m11;  m12 = m12;  m13 = m13;
        m[2][0] = m20;  m[2][1] = m21;  m22 = m22;  m23 = m23;
        m[3][0] = m30;  m[3][1] = m31;  m32 = m32;  m33 = m33;
    }

    void Matrix4::makeIdMatrix() {
        // makes an identity matrix
        m[0][0] = 1.0;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = 0.0;   
        m[1][0] = 0.0;  m[1][1] = 1.0;  m[1][2] = 0.0;  m[1][3] = 0.0;   
        m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] = 1.0;  m[2][3] = 0.0;   
        m[3][0] = 0.0;  m[3][1] = 0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;   
    }

    void Matrix4::makeTranslationMatrix(double dx, double dy, double dz) {
        // makes a translation matrix
        m[0][0] = 1.0;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = dx;   
        m[1][0] = 0.0;  m[1][1] = 1.0;  m[1][2] = 0.0;  m[1][3] = dy;   
        m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] = 1.0;  m[2][3] = dz;   
        m[3][0] = 0.0;  m[3][1] = 0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;   
    }

    void Matrix4::makeScaleMatrix(double sx, double sy, double sz){
        // makes a scale matrix
        m[0][0] =  sx;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = 0.0;   
        m[1][0] = 0.0;  m[1][1] =  sy;  m[1][2] = 0.0;  m[1][3] = 0.0;   
        m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] =  sz;  m[2][3] = 0.0;   
        m[3][0] = 0.0;  m[3][1] = 0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;   
    }

    // makes rotation matrix about X-axis based on a given angle in degrees
    void Matrix4::makeRotationMatrixX(double angle) {
        double angleInRads = angle * DTOR;
        makeRotationMatrixX(sin(angleInRads), cos(angleInRads));
    }

    void Matrix4::makeRotationMatrixX(double sinA, double cosA){
        // makes rotation matrix about X-axis
        m[0][0] = 1.0; m[0][1] =   0.0;  m[0][2] =   0.0;  m[0][3] = 0.0;   
        m[1][0] = 0.0; m[1][1] =  cosA;  m[1][2] = -sinA;  m[1][3] = 0.0;   
        m[2][0] = 0.0; m[2][1] =  sinA;  m[2][2] =  cosA;  m[2][3] = 0.0;   
        m[3][0] = 0.0; m[3][1] =   0.0;  m[3][2] =   0.0;  m[3][3] = 1.0;   
    }

    // makes rotation matrix about Y-axis based on a given angle in degrees
    void Matrix4::makeRotationMatrixY(double angle) {
        double angleInRads = angle * DTOR;
        makeRotationMatrixY(sin(angleInRads), cos(angleInRads));
    }

    void Matrix4::makeRotationMatrixY(double sinA, double cosA){
        // makes rotation matrix about Y-axis
        m[0][0] =  cosA; m[0][1] = 0.0;  m[0][2] =  sinA;  m[0][3] = 0.0;   
        m[1][0] =   0.0; m[1][1] = 1.0;  m[1][2] =   0.0;  m[1][3] = 0.0;   
        m[2][0] = -sinA; m[2][1] = 0.0;  m[2][2] =  cosA;  m[2][3] = 0.0;   
        m[3][0] =   0.0; m[3][1] = 0.0;  m[3][2] =   0.0;  m[3][3] = 1.0;   
    }

    // makes rotation matrix about Z-axis based on a given angle in degrees
    void Matrix4::makeRotationMatrixZ(double angle) {
        double angleInRads = angle * DTOR;
        makeRotationMatrixZ(sin(angleInRads), cos(angleInRads));
    }

    void Matrix4::makeRotationMatrixZ(double sinA, double cosA){
        // makes rotation matrix about Z-axis
        m[0][0] =  cosA; m[0][1] = -sinA;  m[0][2] = 0.0;  m[0][3] = 0.0;   
        m[1][0] =  sinA; m[1][1] =  cosA;  m[1][2] = 0.0;  m[1][3] = 0.0;   
        m[2][0] =   0.0; m[2][1] =   0.0;  m[2][2] = 1.0;  m[2][3] = 0.0;   
        m[3][0] =   0.0; m[3][1] =   0.0;  m[3][2] = 0.0;  m[3][3] = 1.0;   
    }

// multiplies matrix m by vector a
Vector3 Multiply(Matrix4 m, Vector3 a) {
    // returns result = m * a
    Vector3 result;
    result.x = m.m[0][0] * a.x + m.m[0][1] * a.y + m.m[0][2] * a.z + m.m[0][3] * a.w;
    result.y = m.m[1][0] * a.x + m.m[1][1] * a.y + m.m[1][2] * a.z + m.m[1][3] * a.w;
    result.z = m.m[2][0] * a.x + m.m[2][1] * a.y + m.m[2][2] * a.z + m.m[2][3] * a.w;
    result.w = m.m[3][0] * a.x + m.m[3][1] * a.y + m.m[3][2] * a.z + m.m[3][3] * a.w;
    return result;
}

// multiplies vector a by matrix m
Vector3 Multiply(Vector3 a, Matrix4 m) {
    // returns result = a * m
    Vector3 result;
    result.x = a.x * m.m[0][0] + a.y * m.m[1][0] + a.z * m.m[2][0] + a.w * m.m[3][0];
    result.y = a.x * m.m[0][1] + a.y * m.m[1][1] + a.z * m.m[2][1] + a.w * m.m[3][1];
    result.z = a.x * m.m[0][2] + a.y * m.m[1][2] + a.z * m.m[2][2] + a.w * m.m[3][2];
    result.w = a.x * m.m[0][3] + a.y * m.m[1][3] + a.z * m.m[2][3] + a.w * m.m[3][3];
    return result;
}

// multiply x X y using loops
Matrix4 Multiply(Matrix4 x, Matrix4 y) {
    int i, j, k;
    Matrix4 result;

    for(i=0; i < 4; i++) {
        for(j=0; j < 4; j++) {
            result.m[i][j] = 0.0;
            for(k=0; k < 4; k++) {
                result.m[i][j] += x.m[i][k] * y.m[k][j];    
            }
        }
    }
    return result;
}
