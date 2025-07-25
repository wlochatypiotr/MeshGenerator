#include "Mesh.h"
#include "MeshGenerator.h"

int main()
{
    Mesh Triangle;
    MeshGenerator::GenTriangleMesh(Triangle);

    MeshGenerator::ExportToOBJ(Triangle, "riangle.obj");

    Mesh Disc;

    MeshGenerator::GenDiscMesh(Disc,1.0f, 30);

    Mesh Petal;
    MeshGenerator::GenPetalMesh(Petal, 3.0f, 1.0f, 20);

    MeshGenerator::ExportToOBJ(Petal, "Petal.obj");

    MeshGenerator::ExportToOBJ(Disc, "Disc.obj");

	Mesh Flower;

	MeshGenerator::GenFlowerMesh(Flower, 1.f,20, 3.0f,1.0f, 20,9);
	MeshGenerator::ExportToOBJ(Flower, "Flower.obj");

	return 0;
}