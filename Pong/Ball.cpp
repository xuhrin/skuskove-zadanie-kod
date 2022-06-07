#include "Ball.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

void Ball::Reset(float angle) {
	mVelocity.x = std::cos(angle) * Ball::Velocity;
	mVelocity.y = std::sin(angle) * Ball::Velocity;
	mPosition.x = 0.F;
	mPosition.y = 0.F;
	mMatrix.SetPosition(mPosition);
}

void Ball::Update(float delta) {
	mPosition.x += mVelocity.x * delta;
	mPosition.y += mVelocity.y * delta;
	mMatrix.SetPosition(mPosition);
}

void Ball::SetRadius(float radius) {
	mRadius = radius;
	mMatrix.SetScale({ radius, radius });
}

void Ball::Render() {
	Mesh::GetMesh(mMeshID).Render();
}

bool Ball::CheckCollision(const Paddle& paddle) {
	const float hDiff = mPosition.x - paddle.GetPosition().x;
	const float hDistance = std::abs(hDiff);
	if (hDistance > (mRadius + paddle.GetScale().x * 0.5F)) {
		return false;
	}

	const float vDiff = mPosition.y - paddle.GetPosition().y;
	const float vDistance = std::abs(vDiff);
	if (vDistance > (mRadius + paddle.GetScale().y * 0.5F)) {
		return false;
	}

	const float vMax = std::abs(paddle.GetScale().y * 0.5F);
	const float vClamped = std::fminf(vDistance, vMax);

	float angle = (M_PI * 0.25F) * (vClamped / vMax);

	if (hDiff < 0.F) {
		angle = vDiff < 0.F ? M_PI + angle : M_PI - angle;
	}
	else if (vDiff < 0.F) {
		angle = (2.F * M_PI) - angle;
	}

	mVelocity.x = std::cos(angle) * Ball::Velocity;
	mVelocity.y = std::sin(angle) * Ball::Velocity;
	return true;
}

bool Ball::CheckBorderCollision(float sizeY) {
	if (mPosition.y + mRadius >= sizeY && mVelocity.y > 0.F) {
		mVelocity.y = -mVelocity.y;
		return true;
	}
	if (mPosition.y - mRadius <= -sizeY && mVelocity.y < 0.F) {
		mVelocity.y = -mVelocity.y;
		return true;
	}
	return false;
}

int Ball::CheckPaddleBorderCollision(float sizeX) {
	if (std::abs(mPosition.x - sizeX) <= mRadius) {
		return 1;
	}
	if (std::abs(mPosition.x + sizeX) <= mRadius) {
		return -1;
	}
	return 0;
}
