#pragma once

#include "Vector2.hpp"
#include "Matrix3.hpp"
#include "Mesh.hpp"

class Paddle {
public:
	enum Direction {
		Up, Down
	};

	static constexpr float Velocity = 4.5F;

	Paddle() = default;

	void Reset(const Vector2& position, const Vector2& scale);

	void Move(float delta, Direction direction, float maxY);

	const Vector2& GetPosition() const {
		return mPosition;
	}

	const Vector2& GetScale() const {
		return mScale;
	}

	const Matrix3& GetMatrix() const {
		return mMatrix;
	}

	float DistanceFromBorderToPoint(const Vector2& point) const;

	void Render() const;

private:
	unsigned mMeshID{ Mesh::BoxID };
	Vector2 mPosition{};
	Vector2 mScale{};
	Matrix3 mMatrix{};
};
