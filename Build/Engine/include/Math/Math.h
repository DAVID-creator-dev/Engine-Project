#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include "Utils/Engine.h"

#define PI 3.14159265358979323846f

inline float DegToRad(float degrees) {
    return degrees * (PI / 180.0f);
}

inline float RadToDeg(float radians) {
	return radians * (180.0f / PI);
}

class ENGINE_API Vector2D {
public:
    float x, y;

    Vector2D() = default; 
    Vector2D(float _x, float _y) : x(_x), y(_y) {}

    void print() const;
    Vector2D InverseVector();
    Vector2D AdditionVector(Vector2D v1, Vector2D v2);
    Vector2D AddScalar(float scalar) const;
    Vector2D MultiplyVectors(Vector2D v) const;
    Vector2D MultiplyByScalar(float scalar) const;
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator+(float scalar) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator*(const Vector2D& other) const;
    Vector2D operator/(float scalar) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(float scalar);
    Vector2D& operator*=(const Vector2D& other);
    Vector2D& operator/=(float scalar);
    float Norm() const;
    float DotProduct(const Vector2D& v1, const Vector2D& v2) const;
};

class ENGINE_API Vector3D {
public:
    float x, y, z;
    Vector3D() = default; 
    Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {
    }

    void print() const;
    Vector3D InverseVector();
    Vector3D AdditionVector(Vector3D v1, Vector3D v2);
    Vector3D AddScalar(float scalar) const;
    Vector3D MultiplyVectors(Vector3D v) const;
    Vector3D MultiplyByScalar(float scalar) const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator+(float scalar) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    Vector3D operator*(const Vector3D& other) const;
    Vector3D operator/(float scalar) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(float scalar);
    Vector3D& operator*=(const Vector3D& other);
    Vector3D& operator/=(float scalar);
    float Norm() const;
	Vector3D Normalize() const;
    float DotProduct(const Vector3D& other) const;
    Vector3D CrossProduct(const Vector3D& other) const;
    static Vector3D Lerp(const Vector3D& a, const Vector3D& b, float t);

    static Vector3D Normalize(Vector3D v);
    static Vector3D CrossProduct(Vector3D v1, Vector3D v2); 
};

class ENGINE_API Vector4D
{
public:
    float x = 0, y = 0, z = 0, w = 0;

    Vector4D() = default;
    //Vector4D(float a, float b, float c, float d);
    //Vector4D(const Vector3D& v, float d = 0);

    Vector4D(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }

    Vector4D(const Vector3D& v, float d)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = d;
    }

    Vector4D& operator/=(const float& a)
    {
        x /= a;
        y /= a;
        z /= a;
        w /= a;
        return *this;
    }

    ~Vector4D() = default;

    //Vector4D& operator/=(const float& a); 
};

class ENGINE_API Transform
{
public:
    Transform() = default; 
    Transform(Vector3D _pos, Vector3D _eulerRot, Vector3D _scale); 
    ~Transform() = default; 

    Vector3D pos;
    Vector3D eulerRot;
    Vector3D scale;
};

class ENGINE_API Mat4 {
public:
    Mat4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  data[3] = e03;
        data[4] = e10;  data[5] = e11;  data[6] = e12;  data[7] = e13;
        data[8] = e20;  data[9] = e21;  data[10] = e22; data[11] = e23;
        data[12] = e30; data[13] = e31; data[14] = e32; data[15] = e33;
    }

    Mat4() {
        for (int i = 0; i < 16; i++)
            data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    float data[16];
    bool isOrthogonal = false;

    void Print();
    void IsOrthogonal();
    static Mat4 Identity();
    static Mat4 SetTranslation(const Vector3D& position);
    static Mat4 SetScale(const Vector3D& position);
    Mat4 MultiplyMatrices(const Mat4& other);
    static Mat4 RotationX(float angle);
    static Mat4 RotationY(float angle);
    static Mat4 RotationZ(float angle);
    Mat4 InvertMatrix();
    Mat4 Transpose() const;
    float* TransposeData() const;
    static Mat4 TRS(Vector3D pos, Vector3D rot, Vector3D scale);
    static Vector3D TransformDirection(const Mat4& matrix, const Vector3D& direction); 

    Mat4 operator*(const Mat4& other) const;
    Vector3D operator*(const Vector3D& other) const;
    Vector4D operator*(const Vector4D& v) const;
};