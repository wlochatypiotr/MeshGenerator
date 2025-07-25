#pragma once

#include "Mesh.h"
#include <string>

class MeshGenerator
{
public:
    //defaults to right triangle
    static void GenTriangleMesh(Mesh& InMesh,
        const Vertex& V1 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
        const Vertex& V2 = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
        const Vertex& V3 = { 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f });


    static void GenDiscMesh(Mesh& InMesh, float Radius, unsigned int Tessellation);

    static void GenPetalMesh(Mesh& InMesh, float Lenght, float Width, unsigned int Tessellation);

    static void GenFlowerMesh(Mesh& InMesh, float CoreRadius, unsigned int CoreTessellation,
    float PetalLenght, float PetalWidth, unsigned int PetalTessellation, unsigned int PetalCount);
    
    
    static void ExportToOBJ(const Mesh& SourceMesh, const std::string& Filename);

private:
    //helper, populates vertex indices
    static void GenTriangle(Mesh& InMesh, unsigned int Idx1, unsigned int Idx2, unsigned int Idx3);

};

