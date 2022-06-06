#pragma once

#include <glad/glad.h>

struct Mesh {
	static constexpr unsigned BallID = 0;
	static constexpr unsigned BoxID = 1;

	GLuint buffers[2];
	unsigned numIndices;

	static const Mesh& GetMesh(unsigned it);

	void Destroy();

	void Render() const;

private:
	static Mesh GenerateBall(unsigned steps);
	static Mesh GenerateBox();
};
