#pragma once

#include "Vector2.hpp"
#include "Matrix3.hpp"
#include "Mesh.hpp"
#include "Paddle.hpp"

class Ball {
public:
	static constexpr float Velocity = 8.F;

	Ball() = default;

	void Reset(float angle);

	void Update(float delta);

	void SetRadius(float radius);

	const Vector2& GetPosition() const {
		return mPosition;
	}

	const Matrix3& GetMatrix() const {
		return mMatrix;
	}

	void Render();

	bool CheckCollision(const Paddle& paddle);

	bool CheckBorderCollision(float sizeY);

	int CheckPaddleBorderCollision(float sizeX);

private:
	unsigned mMeshID{ Mesh::BallID };
	Vector2 mPosition{};
	Vector2 mVelocity{};
	Matrix3 mMatrix{};
	float mRadius{1.F};
};
