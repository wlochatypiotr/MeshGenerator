#include <string>
#include <iostream>
#include <fstream>
#include "MeshGenerator.h"

static const double PI = atan(1) * 4;
using namespace std;

void MeshGenerator::ExportToOBJ(const Mesh& SourceMesh, const std::string& Filename)
{
    std::ofstream Ofs(Filename);
    if (!Ofs)
    {
        return;
    }

    // position, normals
    for (const Vertex& V : SourceMesh.Vertices) 
    {
        Ofs 
            << "v " 
            << V.X << " " 
            << V.Y << " " 
            << V.Z << "\n";

        Ofs 
            << "vn " 
            << V.NX << " " 
            << V.NY << " " 
            << V.NZ << "\n";
    }

    //INDICES
    for (size_t i = 0; i < SourceMesh.Indices.size(); i += 3) 
    {
        //first index start from 1 in OBJ
        const unsigned int Idx1 = SourceMesh.Indices[i] + 1;
        const unsigned int Idx2 = SourceMesh.Indices[i + 1] + 1;
        const unsigned int Idx3 = SourceMesh.Indices[i + 2] + 1;
        Ofs 
            << "f "
            << Idx1 << "/" << Idx1 << "/" << Idx1 << " "
            << Idx2 << "/" << Idx2 << "/" << Idx2 << " "
            << Idx3 << "/" << Idx3 << "/" << Idx3 << "\n";
    }
    Ofs.close();
}

void MeshGenerator::GenTriangle(Mesh& InMesh, unsigned int Idx1, unsigned int Idx2, unsigned int Idx3)
{
    InMesh.Indices.push_back(Idx1);
    InMesh.Indices.push_back(Idx2);
    InMesh.Indices.push_back(Idx3);
}

void MeshGenerator::GenTriangleMesh(Mesh& InMesh, const Vertex& V1, const Vertex& V2, const Vertex& V3)
{
    InMesh.Vertices.push_back(V1);
    InMesh.Vertices.push_back(V2);
    InMesh.Vertices.push_back(V3);

    const size_t Index0 = InMesh.Vertices.size();
    GenTriangle(InMesh, Index0 - 3, Index0 - 2, Index0 - 1);
}

void MeshGenerator::GenDiscMesh(Mesh& InMesh, float Radius, unsigned int Tessellation) 
{
    const size_t CenterIdx= InMesh.Vertices.size();

    //center at 0,0,0
    InMesh.Vertices.emplace_back(0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f);

    //triangle fan vertices
    for (unsigned int i = 0; i <= Tessellation; ++i) {

        const float Angle = 2.0f * static_cast<float>(PI) * static_cast<float>(i) / static_cast<float>(Tessellation);
        float X = Radius * cos(Angle);
        float Y = Radius * sin(Angle);

        InMesh.Vertices.emplace_back(X, Y, 0.0f,  0.0f, 0.0f, 1.0f);

        //handle indices
        if (i > 0)
        {
            GenTriangle(InMesh, CenterIdx, i, i + 1);
        }
    }
}

void MeshGenerator::GenPetalMesh(Mesh& InMesh, float Lenght, float Width, unsigned int Tessellation)
{
    const size_t CenterIdx = InMesh.Vertices.size();
    //center at 0,0,0
    InMesh.Vertices.emplace_back(0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f); 
    
    for (unsigned int i = 0; i <= Tessellation; ++i)
    {
        const float Step = static_cast<float>(i) / static_cast<float>(Tessellation);
        const float Angle = static_cast<float>(PI) * Step;
        //semi elipsis as shape
        const float X = Width / 2 * cos(Angle);
        const float Y = Lenght  * sin(Angle);

        InMesh.Vertices.emplace_back(X, Y, 0.0f,  0.0f, 0.0f, 1.0f);

        if (i > 0)
        {
            GenTriangle(InMesh, CenterIdx, i, i + 1);
        }
    }
}

void MeshGenerator::GenFlowerMesh(Mesh& InMesh, float CoreRadius, unsigned int CoreTessellation,
    float PetalLenght, float PetalWidth, unsigned int PetalTessellation, unsigned int PetalCount)
{
    GenDiscMesh(InMesh, CoreRadius, CoreTessellation);
    
    // template petal used for generating rotated versions
    Mesh PetalTemplate;
    GenPetalMesh(PetalTemplate, PetalLenght, PetalWidth, PetalTessellation);

    for (unsigned int i = 0; i < PetalCount; ++i)
    {
        // angle per petal
        const float AngleStep = static_cast<float>(PI) * 2.0f * static_cast<float>(i) / static_cast<float>(PetalCount);
        // for rotation
        const float SinAngle = sin(AngleStep);
        const float CosAngle = cos(AngleStep);

        //index for first triangle of new mesh
        const unsigned int NextBase = InMesh.Vertices.size();

        //  Scale -> Rotate -> Transform
        for (const Vertex& V : PetalTemplate.Vertices)
        {
            //move base
            const float YTranslated =  V.Y + CoreRadius;

            //rotate along z-axis
            const float XRotated = V.X * CosAngle - YTranslated * SinAngle;
            const float YRotated = V.X * SinAngle + YTranslated * CosAngle;
            // z stays the same

            //normals
            const float NXRotated = V.NX * CosAngle - V.NY * SinAngle;
            const float NYRotated =  V.NX * SinAngle + V.NY * CosAngle;
            //NZ stays the same

            InMesh.Vertices.emplace_back(XRotated, YRotated, V.Z, NXRotated, NYRotated, V.NZ);
        }

        //add indices
        for (const unsigned int& Idx : PetalTemplate.Indices)
        {
            InMesh.Indices.push_back(Idx + NextBase);
        }
    }
}


