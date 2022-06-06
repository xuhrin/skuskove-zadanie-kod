#include "Paddle.hpp"

#include <cmath>

void Paddle::Reset(const Vector2& position, const Vector2& scale)
{
	mPosition = position;
	mScale = scale;

	mMatrix.SetPosition(mPosition);
	mMatrix.SetScale(mScale);
}

void Paddle::Move(float delta, Direction direction, float maxY)
{
	if (direction == Direction::Up) {
		mPosition.y += Velocity * delta;
	} else if (direction == Direction::Down) {
		mPosition.y -= Velocity * delta;
	}

	maxY -= mScale.y * 0.5F;
	mPosition.y = std::fmaxf(std::fminf(mPosition.y, maxY), -maxY);

	mMatrix.SetPosition(mPosition);
}

float Paddle::DistanceFromBorderToPoint(const Vector2& point) const
{
	float dx = std::abs(point.x - mPosition.x) - mScale.x * 0.5F;
	float dy = std::abs(point.y - mPosition.y) - mScale.y * 0.5F;
	float mx = std::fmaxf(dx, 0.F);
	float my = std::fmaxf(dy, 0.F);
	return std::sqrt(mx * mx + my * my) + std::fminf(std::fmaxf(dx, dy), 0.F);
}

void Paddle::Render() const
{
	Mesh::GetMesh(mMeshID).Render();
}
