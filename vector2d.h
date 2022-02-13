#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>
using namespace std;

class Vector {
public:   
    float x, y;

    // constructors 
    Vector(float i, float j) {
        x = i; y = j;
    }
    Vector() {
        x = 0; y = 0;
    }

    void normalize() {
        float modulus = sqrt(x*x + y*y);
        x = x / modulus;
        y = y / modulus;
    }
    void setMag(float c) {
        this->normalize();
        this->x = x * c;
        this->y = y * c;
    }
    void div(float c) {
        // divide the components of a vector c
        this->x /= c;
        this->y /= c;
    }


    // Operator overloading
    Vector operator+ (Vector z) {
        Vector tmp;
        tmp.x = x + z.x;
        tmp.y = y + z.y;
        return tmp;
    }
    Vector operator- (Vector z) {
        Vector tmp;
        tmp.x = x - z.x;
        tmp.y = y - z.y;
        return tmp;
    }
    Vector operator* (float c) {
        Vector tmp;
        tmp.x = c * x;
        tmp.y = c * y;
        return tmp; 
    }

    
    friend ostream& operator<<(ostream& os, const Vector& x);
};

ostream& operator<<(ostream& os, const Vector& x) {
    os << "(" << x.x << ", " << x.y << ")";
    return os;
}

Vector operator*(float a, Vector b) {
    return b*a;
}


float distance(Vector a, Vector b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

#endif