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
	if (paddle.DistanceFromBorderToPoint(mPosition) <= mRadius) {
		const float dY = mPosition.y - paddle.GetPosition().y;
		const float max = std::abs(paddle.GetScale().y * 0.5F);
		const float dmY = std::fminf(std::abs(dY), max);

		float angle = (M_PI * 0.25F) * (dmY / max);

		if (mVelocity.x >= 0.F) {
			angle = dY < 0.F ? M_PI + angle : M_PI - angle;
		}
		else if (dY < 0.F) {
			angle = (2.F * M_PI) - angle;
		}

		mVelocity.x = std::cos(angle) * Ball::Velocity;
		mVelocity.y = std::sin(angle) * Ball::Velocity;
		return true;
	}
	return false;
}

bool Ball::CheckBorderCollision(float sizeY) {
	if (std::abs(mPosition.y - sizeY) <= mRadius || std::abs(mPosition.y + sizeY) <= mRadius) {
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
