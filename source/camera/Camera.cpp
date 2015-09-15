#include "camera\Camera.h"

void Camera::Slide(const float hDistance, const float vDistance)
{
	mPosition += glm::vec3(hDistance, vDistance, 0);
	UpdateView();
}

void Camera::Move(const float distance)
{
	mPosition += glm::vec3(0, 0, distance);
	UpdateView();
}

bool Camera::StartupPerspective(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	return SetView(position, target, up);
}

void Camera::Update(const float deltaTime)
{

}

const glm::mat4 Camera::GetWorldTransform()
{
	return glm::inverse(mViewTransform);
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

void Camera::UpdateView()
{
	mViewTransform = glm::lookAt(mPosition, mTarget, mUpVector);
	mWorldTransform = glm::inverse(mViewTransform);
	mProjectionViewTransform = mProjectionTransform * mViewTransform;
}

bool Camera::SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	mPosition = position;
	mTarget = target;
	mUpVector = up;

	mViewTransform = glm::lookAt(mPosition, mTarget, mUpVector);

	mWorldTransform = glm::inverse(mViewTransform);

	return true;
}