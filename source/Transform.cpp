#include "Transform.h"

Transform::Transform(float a0, float a1, float a2, float a3, float b0, float b1, float b2, float b3, float c0, float c1, float c2, float c3, float d0, float d1, float d2, float d3)
{
	mData[0][0] = a0;
	mData[0][1] = a1;
	mData[0][2] = a2;
	mData[0][3] = a3;

	mData[1][0] = b0;
	mData[1][1] = b1;
	mData[1][2] = b2;
	mData[1][3] = b3;

	mData[2][0] = c0;
	mData[2][1] = c1;
	mData[2][2] = c2;
	mData[2][3] = c3;

	mData[3][0] = d0;
	mData[3][1] = d1;
	mData[3][2] = d2;
	mData[3][3] = d3;
}

Transform Transform::GetIdentity()
{
	return Transform();
}
