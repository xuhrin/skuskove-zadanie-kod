#pragma once

#include "Vector2.hpp"

struct Matrix3 {
	Matrix3() : Matrix3(1.F, 0.F, 0.F, 0.F, 1.F, 0.F, 0.F, 0.F, 1.F) {}

	Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) : mx{ m00, m01, m02, m10, m11, m12, m20,m21, m22 } {}

	Matrix3(const Matrix3&) = default;
	Matrix3& operator=(const Matrix3&) = default;

	Matrix3 operator*(const Matrix3& m) const {
		return {
				mx[0] * m.mx[0] + mx[1] * m.mx[3] + mx[2] * m.mx[6],
				mx[0] * m.mx[1] + mx[1] * m.mx[4] + mx[2] * m.mx[7],
				mx[0] * m.mx[2] + mx[1] * m.mx[5] + mx[2] * m.mx[8],
				mx[3] * m.mx[0] + mx[4] * m.mx[3] + mx[5] * m.mx[6],
				mx[3] * m.mx[1] + mx[4] * m.mx[4] + mx[5] * m.mx[7],
				mx[3] * m.mx[2] + mx[4] * m.mx[5] + mx[5] * m.mx[8],
				mx[6] * m.mx[0] + mx[7] * m.mx[3] + mx[8] * m.mx[6],
				mx[6] * m.mx[1] + mx[7] * m.mx[4] + mx[8] * m.mx[7],
				mx[6] * m.mx[2] + mx[7] * m.mx[5] + mx[8] * m.mx[8]
		};
	}

	static Matrix3 Ortho(float h, float v)
	{
		return {
			1.F / h,
			0.F,
			0.F,
			0.F,
			1.F / v,
			0.F,
			0.F,
			0.F,
			1.F
		};
	}

	void SetScale(const Vector2& v) {
		mx[0] = v.x;
		mx[4] = v.y;
	}

	void SetPosition(const Vector2& v)
	{
		mx[6] = v.x;
		mx[7] = v.y;
	}

	const float* GetPtr() const {
		return static_cast<const float*>(mx);
	}

	float mx[9];
};
