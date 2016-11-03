#pragma once
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "glm\gtx\transform.hpp"


/*
struct Projection
{
	glm::mat4 projection;
};

struct Orthographic : public Projection
{
	Orthographic(const float left, const float right, const float bottom, const float top)
	{
		projection = glm::ortho(left, right, bottom, top);
	}
};

struct Perspective : public Projection
{
	Perspective(const float fov, const float aspectRatio, const float a_near, const float a_far)
	{
		projection = glm::perspective(fov, aspectRatio, a_near, a_far);
	}
};
*/


class Camera
{
public:
	void Rotate(const float hAngle, const float vAngle);
	void Slide(const float hDistance, const float vDistance);
	void Move(const float distance);

	bool SetPerspectiveProjection(const float fov, const float aspectRatio, const float a_near, const float a_far);
	bool SetOrthographicProjection(const float left, const float right, const float bottom, const float top);

	void Update(const float deltaTime);

	bool SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up);

	const glm::mat4 GetWorldTransform();

	const glm::mat4 GetView();
	const glm::mat4 GetProjection();
	const glm::mat4 GetViewProjection();

	const glm::vec3 GetPosition() { return mPosition; }


protected:
	glm::vec3 mPosition, mTarget, mUpVector;
	float mLeft, mRight, mBottom, mTop;

	//not sure need this, and can calculate on fly anyway
	glm::mat4 mWorldTransform = glm::mat4();//logical update

	glm::mat4 mViewTransform = glm::mat4();//draw transform
	glm::mat4 mProjectionTransform = glm::mat4();
	glm::mat4 mProjectionViewTransform = glm::mat4(); //projectionview * projectionTransform - draw transform
	void UpdateProjectViewTransform();
	void UpdateView();
	bool SetView(const float left, const float right, const float bottom, const float top);
	
private:
	void UpdateFlyCamControls();
};