#ifndef __MeshObject_H__
#define __MeshObject_H__

struct MinMaxVerts
{
	float MinX;
	float MinY;
	float MinZ;
	float MaxX;
	float MaxY;
	float MaxZ;
	float Width;
	float Height;
	float Length;
};

class MeshObject
{
public:
	VertDef* m_pVerts;
	GLushort* m_pIndices;
    int m_NumIndices;
	MinMaxVerts* m_Size;

public:
    MeshObject();
    virtual ~MeshObject();

    void LoadFromOBJ(char* filename, float pScale);
};

#endif
