#ifndef  UTILS_H
#define UTILS_H

#define PI 3.141592f
#define WORKING_WIDTH 1920
#define WORKING_HEIGHT 1080
#define NB_LEVEL 4
#include "stdafx.h"

enum OriginePos
{
	TOP_LEFT,
	CENTER,
	CENTER_LEFT,
	CENTER_FOOT,
	DOWN_LEFT
};

class Utils
{
public:

	static sf::Vector2f CartesianToIsometric(const float _x, const float _y, const float _z, const float _size);

	static sf::Vector2f CartesianToIsometricNotEquilateral(const float _x, const float _y, const float _z, const float _size);

	static sf::Vector2f IsometricToCartesian(const float _x, const float _y, const float _z, const float _size);

	static sf::Vector2f IsometricToCartesianNotEquilateral(const float _x, const float _y, const float _size);

	static sf::Sprite LoadSprite(const std::string _sNom, const OriginePos _originePos, int nbFrameX = 1, int nbFrameY = 1);

	static sf::Sprite CreateSprite(sf::Texture& _texture, const sf::Vector2f _pos, const bool _isCentered = true, const sf::Vector2f _scale = sf::Vector2f(1.f, 1.f)/* = ScaleToScreen(1.f, 1.f)*/, const float _rotation = 0.f);

	static sf::Sprite CreateSpriteWithAnims(sf::Texture& _texture, const sf::Vector2i _nbFramesXY, const bool _isCentered, const sf::Vector2f _scale, const sf::Vector2f _pos);

	static void SetCenteredString(sf::Text& _text, std::string _string);

	static void CreateText(sf::Text& _text, const sf::Font& _font, std::string _string, const unsigned char _size, const sf::Color _color, const bool _isCentered = true);

	static void CreateRectangleShape(sf::RectangleShape& _rect, const sf::Vector2f _pos/* = { GetScreen().width / 2.f, GetScreen().height / 2.f }*/, const sf::Vector2f _size = { 100.f, 100.f }, const sf::Color _color = sf::Color::White, const bool _isCentered = true);

	static void CreateCircleShape(sf::CircleShape& _circle, const sf::Vector2f _pos /*= { GetScreen().width / 2.f, GetScreen().height / 2.f }*/, const float _rad = 50.f, const bool _isCentered = true, const sf::Color _color = sf::Color::White);

	static void BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, sf::RenderTarget& _window, const float _angle = 0.f, const sf::RenderStates _blendMode = sf::BlendAlpha);

	static void BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, sf::RenderTarget& _window,sf::Shader& _shader, const float _angle = 0.f, const sf::BlendMode _blendMode = sf::BlendAdd);

	static void BlitSprite(sf::Sprite _sprite, const sf::Vector2f _pos, const float _angle, sf::RenderTarget& _window);

	static void BlitRectangleShape(sf::RectangleShape _rect, const sf::Vector2f _pos, sf::RenderWindow& _window, const float _angle);

	static sf::String ToString(int _int);

	static std::string ConvertKeyToString(const sf::Event::TextEvent& _text);

	static bool IsCircleContains(sf::CircleShape& _c1, const sf::Vector2f _p1);

	static bool CollisionCirleCircle(sf::CircleShape& _c1, sf::CircleShape& _c2);

	static bool IsCollidingAABBs(const sf::FloatRect& _rect1, const sf::FloatRect& _rect2);

	static bool RectContains(const sf::FloatRect& _rect, const sf::Vector2f _point);

	static sf::Vector2f GetRectangleLengthAxis(sf::RectangleShape& rect);

	static sf::Vector2f GetRectangleHeightAxis(sf::RectangleShape& rect);

	static sf::Vector2f* GetRectangleCorners(sf::RectangleShape& rect);

	static bool OBBContains(sf::RectangleShape& rect, const sf::Vector2f pos);

	static bool SpriteContainsPixelPerfect(sf::Image& _image, const sf::Vector2f _imagePos, const sf::Vector2f _point);

	static bool CircleSegmentCollision(sf::CircleShape& circle, const sf::Vector2f point1, const sf::Vector2f point2);

	static bool CircleRectangleCollision(sf::CircleShape& circle, sf::RectangleShape& rect);

	static sf::Vector2f GetVector(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static sf::Vector2f GetNormalVector(const sf::Vector2f& vector);

	static float SquaredDistance2f(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float GetVectorLength(const sf::Vector2f& vector);

	static float GetSquaredLength(const sf::Vector2f& vector);

	static float GetPointDistance(const sf::Vector2f& _p1, const sf::Vector2f& _p2);

	static float GetSquaredPointDistance(const sf::Vector2f& _p1, const sf::Vector2f& _p2);

	static float DotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);

	static float AngleBetween(const sf::Vector2f& _v1, const sf::Vector2f& _v2);

	static sf::Vector2f NormalizeVector(const sf::Vector2f& vector);

	static sf::Vector2f RotateVector(const sf::Vector2f& vector, const float& angle);

	static float DegreesToRadians(const float& angle);

	static float RadiansToDegrees(const float& angle);

	static float GetSquaredValue(const float& _x);

	static float GetCubedValue(const float& _x);

	static float Clamp(const float& _value, const int& _min, const int& _max);

	static float ClampLength(const sf::Vector2f& _vector, const float& _maxLength);

	static bool IsInBetween(const float& _value, const int& _min, const int& _max);

	static bool IsPointInVisionCone(const sf::Vector2f& _point, const sf::Vector2f& _enemyPos, const sf::Vector2f& _visionVect, const float& _angle);

	static int RandomMinMax(const int& _min, const int& _max);

	static bool AABB_AABB_Collision(sf::RectangleShape* aabb1, sf::RectangleShape* aabb2);

	static float Distance(sf::Vector2f _pos1, sf::Vector2f _pos2);

private:
	Utils();
};
#endif 

