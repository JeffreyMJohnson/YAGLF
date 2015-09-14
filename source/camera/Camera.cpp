#include "camera\Camera.h"

bool Camera::SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	mPosition = position;
	mTarget = target;
	mUpVector = up;

	mViewTransform = glm::lookAt(mPosition, mTarget, mUpVector);

	mWorldTransform = glm::inverse(mViewTransform);

	return true;
}

void Camera::Update(const float deltaTime)
{

}



void Camera::SetLookAt(const glm::vec3 from, const glm::vec3 to, const glm::vec3 up)
{
	mViewTransform = glm::lookAt(from, to, up);
	mWorldTransform = glm::inverse(mViewTransform);
}

void Camera::SetPosition(const glm::vec3 position)
{
	SetView(position, mTarget, mUpVector);

	//mWorldTransform = glm::translate(mWorldTransform, position);
	//mViewTransform = glm::inverse(mWorldTransform);
	UpdateProjectViewTransform();
}

const glm::mat4 Camera::GetWorldTransform()
{
	return glm::inverse(mViewTransform);
	//return mWorldTransform;
}

const glm::mat4 Camera::GetView()
{
	return mViewTransform;
}

const glm::mat4 Camera::GetProjection()
{
	return mProjectionTransform;
}

const glm::mat4 Camera::GetViewProjection()
{
	return mProjectionViewTransform;
}

void Camera::UpdateProjectViewTransform()
{
	mProjectionViewTransform = mProjectionTransform * mViewTransform;
}