#include "CommonHeader.h"

MeshObject::MeshObject()
{
    m_pVerts = 0;
    m_pIndices = 0;
    m_NumIndices = 0;
	m_Size = 0;
}

MeshObject::~MeshObject()
{
    SAFE_DELETE( m_pVerts );
    SAFE_DELETE( m_pIndices );
}

void MeshObject::LoadFromOBJ(char* filename, float pScale)
{
	objLoader mesh;
	mesh.load( filename );

	int numbVerts = mesh.faceCount * 3;

	m_pVerts = new VertDef[numbVerts];
	//m_pNormal = 
	m_pIndices = new GLushort[numbVerts];

	m_Size = new MinMaxVerts;

	m_Size->MaxX = 0;
	m_Size->MaxY = 0;
	m_Size->MaxZ = 0;
	m_Size->MinX = 0;
	m_Size->MinY = 0;
	m_Size->MinZ = 0;
	m_Size->Width = 0;
	m_Size->Height = 0;
	m_Size->Length = 0;

	for(int i=0; i<mesh.faceCount; i++)
	{
		obj_face* pFace = mesh.faceList[i];

		for(int j = 0; j < 3; j++)
		{
			for(int c = 0; c < 3; c++)
			{
				m_pVerts[i*3+j].pos[c] = mesh.vertexList[pFace->vertex_index[j]]->e[c]*pScale;

				if( mesh.textureCount > 0 )
				{
					if(c != 2)
					{
						m_pVerts[i*3+j].uv[c] = mesh.textureList[pFace->texture_index[j]]->e[c];
					}
				}
				else
				{
					m_pVerts[i*3+j].uv[c] = 0;
				}
				
				if( mesh.normalCount > 0 )
					m_pVerts[i*3+j].normal[c] = mesh.normalList[pFace->normal_index[j]]->e[c];
			}
		}

		//m_pIndices[i*3 +0]

		m_pIndices[i*3+0] = i*3+0;
		m_pIndices[i*3+1] = i*3+1;
		m_pIndices[i*3+2] = i*3+2;

	}

	for( int i = 0; i<numbVerts; i++)
	{
		m_pIndices[i] = i;

		if(m_pVerts[i].pos[0] > m_Size->MaxX)
		{
			m_Size->MaxX = m_pVerts[i].pos[0];
		}
		else if(m_pVerts[i].pos[0] < m_Size->MinX)
		{
			m_Size->MinX = m_pVerts[i].pos[0];
		}

		if(m_pVerts[i].pos[1] > m_Size->MaxY)
		{
			m_Size->MaxY = m_pVerts[i].pos[1];
		}
		else if(m_pVerts[i].pos[1] < m_Size->MinY)
		{
			m_Size->MinY = m_pVerts[i].pos[1];
		}

		if(m_pVerts[i].pos[2] > m_Size->MaxZ)
		{
			m_Size->MaxZ = m_pVerts[i].pos[2];
		}
		else if(m_pVerts[i].pos[2] < m_Size->MinZ)
		{
			m_Size->MinZ = m_pVerts[i].pos[2];
		}
	}

	m_Size->Width = m_Size->MaxX - m_Size->MinX;
	m_Size->Height = m_Size->MaxY - m_Size->MinY;
	m_Size->Length = m_Size->MaxZ - m_Size->MinZ;
	
	m_NumIndices = numbVerts;
}
