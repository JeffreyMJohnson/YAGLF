#include "Camera.h"

void Camera::Slide(const float hDistance, const float vDistance)
{
	mPosition += glm::vec3(hDistance, vDistance, 0);
	mTarget += glm::vec3(hDistance, vDistance, 0);
	UpdateView();
}

void Camera::Move(const float distance)
{
	glm::vec3 direction = glm::normalize(mTarget - mPosition);
	mPosition += distance * direction;
	mTarget += distance * direction;
	UpdateView();
}

bool Camera::SetPerspectiveProjection(const float fov, const float aspectRatio, const float near, const float far)
{
	mProjectionTransform = glm::perspective(fov, aspectRatio, near, far);
	UpdateProjectViewTransform();
	return true;
}

void Camera::Update(const float deltaTime)
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		Move(1 * mCamSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		Move(-1 * mCamSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		Slide(-1 * mCamSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		Slide(1 * mCamSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		Slide(0, 1 * mCamSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		Slide(0, -1 * mCamSpeed * deltaTime);
	}
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
	UpdateProjectViewTransform();
}

bool Camera::SetView(const glm::vec3 position, const glm::vec3 target, const glm::vec3 up)
{
	mPosition = position;
	mTarget = target;
	mUpVector = up;

	UpdateView();

	return true;
}