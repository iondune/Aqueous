#ifndef _CABBAGE_SCENE_CMESH_H_INCLUDED_
#define _CABBAGE_SCENE_CMESH_H_INCLUDED_

#include "../CabbageCore/SBoundingBox3.h"

#include "SVertex.h"
#include "CBufferObject.h"
#include "CMaterial.h"

#include <vector>

class CMesh
{

	bool Dirty;
	unsigned int Revision;

public:

    struct STriangle
    {
        unsigned int Indices[3];
        SVector3 Normal;
    };

    struct SMeshBuffer
    {
        std::vector<SVertex> Vertices;
        std::vector<STriangle> Triangles;

		CBufferObject<float> PositionBuffer, ColorBuffer, NormalBuffer, TangentBuffer, BinormalBuffer, TexCoordBuffer, NormalLineBuffer, NormalColorBuffer;
		CBufferObject<unsigned short> IndexBuffer, NormalIndexBuffer;

		void updateBuffers();

        CMaterial Material;
    };

    std::vector<SMeshBuffer *> MeshBuffers;

    CMesh();
    ~CMesh();

    unsigned int const getVertexCount() const;

    void centerMeshByAverage(SVector3 const & CenterLocation);
    void centerMeshByExtents(SVector3 const & CenterLocation);

    void resizeMesh(SVector3 const & Scale);

    SVector3 const getExtent() const;

    void calculateNormalsPerFace();
    void calculateNormalsPerVertex(bool CombineNear = true, float const NearTolerance = 0.01f);

    void updateBuffers();

    SBoundingBox3 const getBoundingBox() const;

    void linearizeIndices();

	bool const isDirty() const;
	void setDirty(bool const dirty);

	unsigned int const getRevision() const;

};

#endif
