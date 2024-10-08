#ifndef MATH_HPP
#define MATH_HPP

#include "Common.h"
#include "stdafx.h"

class Point
{

public:

	Point(float _x, float _y, float _z);
	float GetX();
	float GetY();
	float GetZ();
	void SetX(float _x);
	void SetY(float _y);
	void SetZ(float _z);

private:

	float x;
	float y;
	float z;

};

class Vector
{

public:

	Vector(Point _A, Point _B);
	Vector(float _x, float _y, float _z);
	float GetX();
	float GetY();
	float GetZ();
	float GetNormalize();
	float GetSquaredNormalize();
	float GetScalarProduct(Vector _V2);
	float GetAngle(Vector _V2);
	Vector GetUnitVector();
	void SetRotation2D(float _angle);
	void SetX(float _x);
	void SetY(float _y);
	void SetZ(float _z);
	static float GetVectorLength(sf::Vector2f vector);
	static sf::Vector2f CreateVector(sf::Vector2f _vector1, sf::Vector2f _vector2);

	static sf::Vector2f NormalizeVector(sf::Vector2f vector);

private:

	float x;
	float y;
	float z;

};

#endif