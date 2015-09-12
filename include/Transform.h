#pragma once

class Transform
{
public:
	Transform() : Transform(0){}

	Transform(float value) :
		Transform(
			value, value, value, value,
			value, value, value, value,
			value, value, value, value,
			value, value, value, value) {};

	Transform(
		float a0, float a1, float a2, float a3,
		float b0, float b1, float b2, float b3,
		float c0, float c1, float c2, float c3,
		float d0, float d1, float d2, float d3
		);
	Transform GetIdentity();
private:
	float mData[4][4];
};