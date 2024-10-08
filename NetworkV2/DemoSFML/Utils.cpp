#include "stdafx.h"
#include "Utils.h"

sf::Vector2f Utils::CartesianToIsometric(const float _x, const float _y, const float _z, const float _size)
{
	return sf::Vector2f
	(
		(float)((_x - _y) * _size / 2.f),
		(float)((_x + _y) * _size / 3.5f - _z * _size * 0.54687f)
	);
}

sf::Vector2f Utils::CartesianToIsometricNotEquilateral(const float _x, const float _y, const float _z, const float _size)
{
	return sf::Vector2f(
		(float)((_x - _y) * _size / 2),
		(float)((_x + _y) * _size / 4 - _z * _size * 0.625f)
	);
}

sf::Vector2f Utils::IsometricToCartesian(const float _x, const float _y, const float _z, const float _size)
{
	float X = (7 * _y + 4 * _x + 3.85f * _size * _z) / (4 * _size);
	float Y = X - 2 * _x / _size;

	return sf::Vector2f(X, Y);
}

sf::Vector2f Utils::IsometricToCartesianNotEquilateral(const float _x, const  float _y, const float _size)
{
	return sf::Vector2f
	(
		(_y / (float)(_size / 2.f) + _x / (float)(_size)),
		(_y / (float)(_size / 2.f) - _x / (float)(_size))
	);
}

sf::Sprite Utils::LoadSprite(const std::string _sNom, const OriginePos _originePos, int nbFrameX, int nbFrameY)
{
	sf::Sprite tempSprite;
	sf::Texture* tempTexture = new sf::Texture();
	tempTexture->loadFromFile(_sNom);
	tempTexture->setSmooth(true);
	tempSprite.setTexture(*tempTexture);
	tempSprite.setScale(1.f, 1.f);
	sf::Vector2u tempTaille;
	sf::Vector2f nouvelleTaille;

	switch (_originePos)
	{
	case TOP_LEFT:
		break;
	case CENTER:
		tempTaille = tempTexture->getSize();
		nouvelleTaille = { (float)((tempTaille.x / nbFrameX) / 2.f), (float)((tempTaille.y/ nbFrameY) / 2.f) };
		tempSprite.setOrigin(nouvelleTaille);
		break;
	case CENTER_FOOT:
		tempTaille = tempTexture->getSize();
		nouvelleTaille = { (float)((tempTaille.x / nbFrameX) / 2.f), (float)((tempTaille.y / nbFrameY))};
		tempSprite.setOrigin(nouvelleTaille);
		break;
	case CENTER_LEFT:
		tempTaille = tempTexture->getSize();
		nouvelleTaille = { (float)((tempTaille.x / nbFrameX) / 2.f), 0};
		tempSprite.setOrigin(nouvelleTaille);
		break;
	case DOWN_LEFT:
		tempTaille = tempTexture->getSize();
		nouvelleTaille = { 0,(float)tempTaille.y};
		tempSprite.setOrigin(nouvelleTaille);
		break;
	}
	sf::IntRect rect = sf::IntRect(0, 0, tempSprite.getTexture()->getSize().x / nbFrameX, tempSprite.getTexture()->getSize().y/ nbFrameY);
	tempSprite.setTextureRect(rect);
	return tempSprite;
}

void Utils::BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, sf::RenderTarget& _window, const float _angle, const sf::RenderStates _blendMode)
{
	_sprite.setPosition(_pos);
	_sprite.setRotation(_angle);
	_window.draw(_sprite, _blendMode);
}

void Utils::BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, sf::RenderTarget& _window, sf::Shader& _shader, const float _angle, const sf::BlendMode _blendMode)
{
	_sprite.setPosition(_pos);
	_sprite.setRotation(_angle);
	sf::Transform transform = sf::Transform();
	sf::Texture texture = sf::Texture();
	sf::RenderStates render = (_blendMode, transform, texture, &_shader);
	_window.draw(_sprite, render);

}

void Utils::BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, const float _angle, sf::RenderTarget& _window)
{
	_sprite.setPosition(_pos);
	_sprite.setRotation(_angle);
	_window.draw(_sprite);
}

void Utils::BlitRectangleShape(sf::RectangleShape _rect, const sf::Vector2f _pos, sf::RenderWindow& _window, const float _angle)
{
	_rect.setPosition(_pos);
	_rect.setRotation(_angle);
	_window.draw(_rect);
}

sf::Sprite Utils::CreateSprite(sf::Texture& _texture, const sf::Vector2f _pos, const bool _isCentered, const sf::Vector2f _scale, const float _rotation)
{
	sf::Sprite temp;
	_texture.setSmooth(true);
	temp.setTexture(_texture);
	temp.setScale(_scale);
	if (_isCentered)
	{
		temp.setOrigin((float)(_texture.getSize().x) / 2.f, (float)(_texture.getSize().y) / 2.f);
	}
	temp.setPosition(_pos);
	temp.setRotation(_rotation);


	return temp;
}

sf::Sprite Utils::CreateSpriteWithAnims(sf::Texture& _texture, const sf::Vector2i _nbFramesXY, const bool _isCentered, const sf::Vector2f _scale, const sf::Vector2f _pos)
{
	sf::Sprite* temp = new sf::Sprite;
	temp->setTexture(_texture);
	if (_isCentered)
	{
		temp->setOrigin(_texture.getSize().x / (2.f * _nbFramesXY.x), _texture.getSize().y / (2.f * _nbFramesXY.y));
	}
	temp->setPosition(_pos);
	temp->setScale(_scale);


	return (*temp);
}

void Utils::CreateRectangleShape(sf::RectangleShape& _rect, const sf::Vector2f _pos, const sf::Vector2f _size, const sf::Color _color, const bool _isCentered)
{
	_rect.setSize(_size);
	if (_isCentered)
	{
		_rect.setOrigin(sf::Vector2f(_size.x / 2.f, _size.y / 2.f));
	}
	_rect.setPosition(_pos);
	_rect.setFillColor(_color);
	_rect.setOutlineColor(sf::Color::Magenta);
}

void Utils::CreateCircleShape(sf::CircleShape& _circle, const sf::Vector2f _pos, const float _rad, const bool _isCentered, const sf::Color _color)
{
	_circle.setRadius(_rad);
	if (_isCentered)
	{
		_circle.setOrigin(_rad / 2.f, _rad / 2.f);
	}
	_circle.setPosition(_pos);
	_circle.setFillColor(_color);
	_circle.setOutlineColor(sf::Color::Magenta);
}

void Utils::SetCenteredString(sf::Text& _text, std::string _string)
{
	_text.setString(_string);
	_text.setOrigin(sf::Vector2f(_text.getGlobalBounds().width / 2.f, _text.getGlobalBounds().height / 2.f));
}

void Utils::CreateText(sf::Text& _text, const sf::Font& _font, std::string _string, const unsigned char _size, const sf::Color _color, const bool _isCentered)
{
	_text.setFont(_font);
	_text.setCharacterSize(_size);
	_text.setScale(1.f, 1.f);
	if (_isCentered)
	{
		SetCenteredString(_text, _string);
	}
	else
	{
		_text.setString(_string);
	}
	_text.setFillColor(_color);
}

sf::String Utils::ToString(int _int)
{
	char numstr[10];
	sprintf_s(numstr, "%i", _int);
	return numstr;
}

std::string Utils::ConvertKeyToString(const sf::Event::TextEvent& _text)
{
	sf::Uint32 unicode = _text.unicode;
	std::string string;
	sf::Utf32::encodeWide(unicode, std::back_inserter(string));

	return string;
}


bool Utils::IsCircleContains(sf::CircleShape& _c1, const sf::Vector2f _p1)
{
	float distX = _c1.getPosition().x - _p1.x;
	float distY = _c1.getPosition().y - _p1.y;
	float distance = distX * distX + distY * distY;
	return distance < _c1.getRadius()* _c1.getRadius();
}

bool Utils::CollisionCirleCircle(sf::CircleShape& _c1, sf::CircleShape& _c2)
{
	return SquaredDistance2f(sf::Vector2f(_c1.getGlobalBounds().left, _c1.getGlobalBounds().top), sf::Vector2f(_c2.getGlobalBounds().left, _c2.getGlobalBounds().top)) <= (_c1.getRadius() + _c2.getRadius()) * (_c1.getRadius() + _c2.getRadius());
}

bool Utils::IsCollidingAABBs(const sf::FloatRect& _rect1, const sf::FloatRect& _rect2)
{
	if (_rect1.left + _rect1.width < _rect2.left || _rect1.left > _rect2.left + _rect2.width || _rect1.top + _rect1.height < _rect2.top || _rect1.top > _rect2.top + _rect2.height)
	{
		return false;
	}

	return true;
}

bool Utils::RectContains(const sf::FloatRect& _rect, const sf::Vector2f _point)
{
	return (_point.x > _rect.left) && (_point.x < _rect.left + _rect.width) && (_point.y > _rect.top) && (_point.y < _rect.top + _rect.height);
}

sf::Vector2f Utils::GetRectangleLengthAxis(sf::RectangleShape& rect)
{
	sf::Vector2f rectSize = rect.getSize();
	float rectRotation = DegreesToRadians(rect.getRotation());

	return sf::Vector2f(rectSize.x * cos(rectRotation), rectSize.x * sin(rectRotation));
}

sf::Vector2f Utils::GetRectangleHeightAxis(sf::RectangleShape& rect)
{
	sf::Vector2f rectSize = rect.getSize();
	float rectRotation = DegreesToRadians(rect.getRotation());

	return sf::Vector2f(-rectSize.y * sin(rectRotation), rectSize.y * cos(rectRotation));
}

sf::Vector2f* Utils::GetRectangleCorners(sf::RectangleShape& rect)
{
	sf::Vector2f* corners = new sf::Vector2f[4];

	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f lengthAxis = GetRectangleLengthAxis(rect);
	sf::Vector2f heightAxis = GetRectangleHeightAxis(rect);

	corners[0] = sf::Vector2f(rectPos.x - lengthAxis.x / 2 - heightAxis.x / 2, rectPos.y - lengthAxis.y / 2 - heightAxis.y / 2);
	corners[1] = sf::Vector2f(rectPos.x - lengthAxis.x / 2 + heightAxis.x / 2, rectPos.y - lengthAxis.y / 2 + heightAxis.y / 2);
	corners[2] = sf::Vector2f(rectPos.x + lengthAxis.x / 2 + heightAxis.x / 2, rectPos.y + lengthAxis.y / 2 + heightAxis.y / 2);
	corners[3] = sf::Vector2f(rectPos.x + lengthAxis.x / 2 - heightAxis.x / 2, rectPos.y + lengthAxis.y / 2 - heightAxis.y / 2);

	return corners;
}

bool Utils::OBBContains(sf::RectangleShape& rect, const sf::Vector2f pos)
{
	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();
	float rectRotation = DegreesToRadians(rect.getRotation());

	float COS = cos(rectRotation);
	float SIN = sin(rectRotation);

	sf::Vector2f lengthAxis(rectSize.x * COS, rectSize.x * SIN);
	sf::Vector2f heightAxis(-rectSize.y * SIN, rectSize.y * COS);

	sf::Vector2f corner(rectPos.x - lengthAxis.x / 2 - heightAxis.x / 2, rectPos.y - lengthAxis.y / 2 - heightAxis.y / 2);
	sf::Vector2f CornerToPos(pos.x - corner.x, pos.y - corner.y);

	float xDot = DotProduct(lengthAxis, CornerToPos);
	float maxDotX = DotProduct(lengthAxis, lengthAxis);

	if (xDot < 0 || xDot > maxDotX)
	{
		return false;
	}

	float yDot = DotProduct(heightAxis, CornerToPos);
	float maxDotY = DotProduct(heightAxis, heightAxis);

	if (yDot < 0 || yDot > maxDotY)
	{
		return false;
	}

	return true;
}


bool Utils::SpriteContainsPixelPerfect(sf::Image& _image, const sf::Vector2f _imagePos, const sf::Vector2f _point)
{
	sf::Color pixColor = _image.getPixel((unsigned int)(_point.x - _imagePos.x), (unsigned int)(_point.y - _imagePos.y));
	return (pixColor.a != 0);
}

bool Utils::CircleSegmentCollision(sf::CircleShape& circle, const sf::Vector2f point1, const sf::Vector2f point2)
{
	//init
	sf::Vector2f P1CircleVect = GetVector(point1, circle.getPosition());
	sf::Vector2f P2CircleVect = GetVector(point2, circle.getPosition());
	sf::Vector2f P1P2Vect = GetVector(point1, point2);

	sf::Vector2f P1ProjectionVect =
	{ DotProduct(P1P2Vect,P1CircleVect) / GetSquaredLength(P1P2Vect) * P1P2Vect.x,
		DotProduct(P1P2Vect,P1CircleVect) / GetSquaredLength(P1P2Vect) * P1P2Vect.y
	};

	sf::Vector2f projectionPoint = { point1.x + P1ProjectionVect.x, point1.y + P1ProjectionVect.y };
	sf::Vector2f CircleProjectionVect = GetVector(circle.getPosition(), projectionPoint);

	//collision tests

	if (GetVectorLength(P1CircleVect) <= circle.getRadius() || GetVectorLength(P2CircleVect) <= circle.getRadius())
	{
		return true;
	}

	if (DotProduct(P1P2Vect, P1ProjectionVect) < 0 || DotProduct(P1P2Vect, P1ProjectionVect) > GetSquaredLength(P1P2Vect))
	{
		return false;
	}

	if (GetSquaredLength(CircleProjectionVect) < circle.getRadius() * circle.getRadius())
	{
		return true;
	}

	return false;
}

bool Utils::CircleRectangleCollision(sf::CircleShape& circle, sf::RectangleShape& rect)
{
	sf::FloatRect rectBounds = rect.getGlobalBounds();
	sf::Vector2f* corners = GetRectangleCorners(rect);

	if (RectContains(rect.getGlobalBounds(), circle.getPosition()))
	{
		delete[] corners;
		return true;
	}

	if (IsCircleContains(circle, rect.getPosition()))
	{
		delete[] corners;
		return true;
	}

	for (unsigned char i = 0; i < 4; i++)
	{
		for (unsigned char j = 0; j < 4; j++)
		{
			if (CircleSegmentCollision(circle, corners[i], corners[j]))
			{
				delete[] corners;
				return true;
			}
		}
	}

	delete[] corners;

	return false;
}

sf::Vector2f Utils::GetVector(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return sf::Vector2f(p2.x - p1.x, p2.y - p1.y);
}

sf::Vector2f Utils::GetNormalVector(const sf::Vector2f& vector)
{
	return sf::Vector2f(-vector.y, vector.x);
}

float Utils::SquaredDistance2f(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return GetSquaredLength(GetVector(p1, p2));
}

float Utils::GetVectorLength(const sf::Vector2f& vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

float Utils::GetSquaredLength(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

float Utils::GetPointDistance(const sf::Vector2f& _p1, const sf::Vector2f& _p2)
{
	return sqrtf(GetSquaredPointDistance(_p1, _p2));
}

float Utils::GetSquaredPointDistance(const sf::Vector2f& _p1, const sf::Vector2f& _p2)
{
	return (_p2.x - _p1.x) * (_p2.x - _p1.x) + (_p2.y - _p1.y) * (_p2.y - _p1.y);
}

float Utils::DotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Utils::AngleBetween(const sf::Vector2f& _v1, const sf::Vector2f& _v2)
{
	sf::Vector2f V1Normalized = NormalizeVector(_v1);
	sf::Vector2f V2Normalized = NormalizeVector(_v2);

	return acosf(DotProduct(V1Normalized, V2Normalized));;
}

sf::Vector2f Utils::NormalizeVector(const sf::Vector2f& vector)
{
	float vectorLength = GetVectorLength(vector);

	if (vectorLength == 0.0f)
	{
		return sf::Vector2f(0.f, 0.f);
	}

	float newX = vector.x / vectorLength;
	float newY = vector.y / vectorLength;

	return sf::Vector2f(newX, newY);
}

sf::Vector2f Utils::RotateVector(const sf::Vector2f& vector, const float& angle)
{
	float COS = cosf(angle);
	float SIN = sinf(angle);

	float newX = vector.x * COS - vector.y * SIN;
	float newY = vector.x * SIN + vector.y * COS;

	return sf::Vector2f(newX, newY);
}

float Utils::DegreesToRadians(const float& angle)
{
	return angle * (float)PI / 180;
}

float Utils::RadiansToDegrees(const float& angle)
{
	return angle * 180 / (float)PI;
}

float Utils::GetSquaredValue(const float& _x)
{
	return _x * _x;
}

float Utils::GetCubedValue(const float& _x)
{
	return _x * _x * _x;
}

float Utils::Clamp(const float& _value, const int& _min, const int& _max)
{
	if (_value > _max)
	{
		return static_cast<float>(_max);
	}
	else if (_value < _min)
	{
		return static_cast<float>(_min);
	}

	return _value;
}

float Utils::ClampLength(const sf::Vector2f& _vector, const float& _maxLength)
{
	if (GetSquaredLength(_vector) > _maxLength * _maxLength)
	{
		return _maxLength;
	}

	return GetVectorLength(_vector);
}

bool Utils::IsInBetween(const float& _value, const int& _min, const int& _max)
{
	if (_value >= _min && _value <= _max)
	{
		return true;
	}

	return false;
}

bool Utils::IsPointInVisionCone(const sf::Vector2f& _point, const sf::Vector2f& _enemyPos, const sf::Vector2f& _visionVect, const float& _angle)
{
	//part 1
	sf::Vector2f enemyPointVector = GetVector(_enemyPos, _point);
	float enemyPointSquaredNorm = GetSquaredLength(enemyPointVector);
	float enemyForwardSquaredNorm = GetSquaredLength(_visionVect);

	//part 2
	sf::Vector2f enemyPointNormalizedVector = NormalizeVector(enemyPointVector);
	sf::Vector2f enemyForwardNormalizedVector = NormalizeVector(_visionVect);

	float angle = acosf(DotProduct(enemyPointNormalizedVector, enemyForwardNormalizedVector));

	if (enemyPointSquaredNorm < enemyForwardSquaredNorm && angle >= 0 && angle <= _angle)
	{
		return true;
	}

	return false;
}

int Utils::RandomMinMax(const int& _min, const int& _max)
{
	return rand() % (_max - _min) + _min;
}

bool Utils::AABB_AABB_Collision(sf::RectangleShape* aabb1, sf::RectangleShape* aabb2)
{
	float xMax1 = aabb1->getGlobalBounds().left + aabb1->getGlobalBounds().width;
	float yMax1 = aabb1->getGlobalBounds().top + aabb1->getGlobalBounds().height;
	float xMin1 = aabb1->getGlobalBounds().left;
	float yMin1 = aabb1->getGlobalBounds().top;

	float xMax2 = aabb2->getGlobalBounds().left + aabb2->getGlobalBounds().width;
	float yMax2 = aabb2->getGlobalBounds().top + aabb2->getGlobalBounds().height;
	float xMin2 = aabb2->getGlobalBounds().left;
	float yMin2 = aabb2->getGlobalBounds().top;

	if (xMax1 < xMin2 || xMin1 > xMax2 || yMax1 < yMin2 || yMin1 > yMax2)
	{
		return false;
	}

	return true;
}

float Utils::Distance(sf::Vector2f _pos1, sf::Vector2f _pos2)
{
	return std::sqrtf((_pos2.x - _pos1.x) * (_pos2.x - _pos1.x) + (_pos2.y - _pos1.y) * (_pos2.y - _pos1.y));
}