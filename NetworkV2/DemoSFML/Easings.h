#ifndef EASINGS_H
#define EASINGS_H

#include "stdafx.h"
#include "Utils.h"


class Easings
{
public:

	static sf::Vector2f Cerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	static sf::Vector2f Lerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	static sf::Vector2f Lerp2(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	static float InSine(const float& _t);

	static float OutSine(const float& _t);

	static float InOutSine(const float& _t);

	static float InQuad(const float& _t);

	static float OutQuad(const float& _t);

	static float InOutQuad(const float& _t);

	static float InCubic(const float& _t);

	static float OutCubic(const float& _t);

	static float InOutCubic(const float& _t);

	static float InQuart(const float& _t);

	static float OutQuart(const float& _t);

	static float InOutQuart(const float& _t);

	static float InQuint(const float& _t);

	static float OutQuint(const float& _t);

	static float InOutQuint(const float& _t);

	static float InExpo(const float& _t);

	static float OutExpo(const float& _t);

	static float InOutExpo(const float& _t);

	static float InCirc(const float& _t);

	static float OutCirc(const float& _t);

	static float InOutCirc(const float& _t);

	static float InBack(const float& _t);

	static float OutBack(const float& _t);

	static float InOutBack(const float& _t);

	static float InElastic(const float& _t);

	static float OutElastic(const float& _t);

	static float InOutElastic(const float& _t);

	static float InBounce(const float& _t);

	static float OutBounce(float _t);

	static float InOutBounce(const float& _t);
};

#endif // !EASINGS_H

