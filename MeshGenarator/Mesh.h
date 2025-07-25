#pragma once
#include <vector>

struct Vertex
{
	// vertex positions
	float X, Y, Z = 0;

	// vertex normals
	float NX, NY, NZ = 0;

	//dropping UVs - not needed

	Vertex (float InX, float InY, float InZ,
		float InNX, float InNY, float InNZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;

		NX = InNX;
		NY = InNY;
		NZ = InNZ;
	}
};

struct Mesh
{
	std::vector<Vertex> Vertices{};
	std::vector<unsigned int> Indices{};
};