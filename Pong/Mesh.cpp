#include "Mesh.hpp"
#include "Vector2.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

static void GenBuffersWithData(Mesh& mesh, const Vector2* vertices, unsigned vcount, const unsigned* indices, unsigned icount) {
	glGenBuffers(2, mesh.buffers);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2) * vcount, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * icount, indices, GL_STATIC_DRAW);

	mesh.numIndices = icount;
}

Mesh Mesh::GenerateBall(unsigned steps) {
	unsigned numVertices = steps + 1;
	unsigned numIndices = steps * 3;

	std::vector<Vector2> vertices(numVertices);
	std::vector<unsigned> indices(numIndices);

	float radius = 1.F;
	float step = (2.F * M_PI) / float(steps);
	float f = 0.F;
	for (unsigned i = 0; i < steps; ++i, f += step) {
		vertices[i].x = std::cos(f) * radius;
		vertices[i].y = std::sin(f) * radius;
	}

	// Last one is center
	unsigned centerIndex = steps;
	vertices[centerIndex] = { 0.F, 0.F };

	for (unsigned i = 0; i < steps; ++i) {
		indices[i * 3 + 0] = centerIndex;
		indices[i * 3 + 1] = i;
		indices[i * 3 + 2] = (i + 1) % steps;
	}

	// Store

	Mesh mesh{};
	GenBuffersWithData(mesh, vertices.data(), vertices.size(), indices.data(), indices.size());
	return mesh;
}

Mesh Mesh::GenerateBox()
{
	static const Vector2 Vertices[] = {
		{-0.5F, -0.5F},
		{-0.5F,  0.5F},
		{ 0.5F,  0.5F},
		{ 0.5F, -0.5F}
	};

	static const unsigned Indices[] = {
		0, 1, 2, 0, 2, 3
	};

	Mesh mesh{};
	GenBuffersWithData(mesh, Vertices, 4, Indices, 6);
	return mesh;
}

const Mesh& Mesh::GetMesh(unsigned id)
{
	static const Mesh Ball = Mesh::GenerateBall(32);
	static const Mesh Block = Mesh::GenerateBox();

	switch (id) {
	case Mesh::BallID:
		return Ball;
	default:
		return Block;
	}
}

void Mesh::Destroy()
{
	glDeleteBuffers(2, buffers);
}

void Mesh::Render() const {
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), nullptr);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

