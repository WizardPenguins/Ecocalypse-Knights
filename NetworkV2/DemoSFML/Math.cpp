#include "stdafx.h"
#include "Math.hpp"

float power(float _nb, int _power)
{

	float res = 1;

	if (_power == 0)
	{

		return 1;

	}

	for (int i = 0; i < _power; i++)
	{

		res *= _nb;

	}

	return res;

}

Point::Point(float _x, float _y, float _z = 0)
{

	this->x = _x;
	this->y = _y;
	this->z = _z;

}

float Point::GetX()
{

	return this->x;

}

float Point::GetY()
{

	return this->y;

}

float Point::GetZ()
{

	return this->z;

}

void Point::SetX(float _x)
{

	this->x = _x;

}

void Point::SetY(float _y)
{

	this->y = _y;

}

void Point::SetZ(float _z)
{

	this->z = _z;

}

Vector::Vector(Point _A, Point _B)
{

	this->x = _B.GetX() - _A.GetX();
	this->y = _B.GetY() - _A.GetY();
	this->z = _B.GetZ() - _A.GetZ();

}

Vector::Vector(float _x, float _y, float _z = 0)
{

	this->x = _x;
	this->y = _y;
	this->z = _z;

}

float Vector::GetX()
{

	return this->x;

}

float Vector::GetY()
{

	return this->y;

}

float Vector::GetZ()
{

	return this->z;

}

float Vector::GetNormalize()
{

	return sqrtf(power(this->GetX(), 2) + power(this->GetY(), 2) + power(this->GetZ(), 2));

}

float Vector::GetSquaredNormalize()
{

	return power(this->GetX(), 2) + power(this->GetY(), 2) + power(this->GetZ(), 2);

}

float Vector::GetScalarProduct(Vector _V2)
{

	return this->GetX() * _V2.GetX() + this->GetY() * _V2.GetY() + this->GetZ() * _V2.GetZ();

}

float Vector::GetAngle(Vector _V2)
{

	return this->GetScalarProduct(_V2) / (this->GetNormalize() * _V2.GetNormalize());

}

Vector Vector::GetUnitVector()
{
	Vector unit(this->GetX() / this->GetNormalize(), this->GetY() / this->GetNormalize(), this->GetZ() / this->GetNormalize());

	return unit;

}

void Vector::SetRotation2D(float _angle)
{

	this->SetX(this->GetX() * cos(_angle) + this->GetY() * sin(_angle));
	this->SetY(-this->GetX() * sin(_angle) + this->GetY() * cos(_angle));

}

void Vector::SetX(float _x)
{

	this->x = _x;

}

void Vector::SetY(float _y)
{

	this->y = _y;

}

void Vector::SetZ(float _z)
{

	this->z = _z;

}

float Vector::GetVectorLength(sf::Vector2f vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f Vector::CreateVector(sf::Vector2f _vector1, sf::Vector2f _vector2)
{
	return { _vector2.x - _vector1.x,_vector2.y - _vector1.y };
}

sf::Vector2f Vector::NormalizeVector(sf::Vector2f vector)
{
	float vectorLength = Vector::GetVectorLength(vector);

	if (vectorLength == 0.0f)
	{
		return sf::Vector2f(0, 0);
	}

	float newX = vector.x / vectorLength;
	float newY = vector.y / vectorLength;

	return sf::Vector2f(newX, newY);
}