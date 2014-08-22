#include "CommonHeader.h"

Net::Net()
{
	mNetBack = new MeshObject();
	mNetFrame = new MeshObject();
	mNetSide1 = new MeshObject();
	mNetSide2 = new MeshObject();
	mNetTop = new MeshObject();
}

Net::~Net()
{
}

void Net::Initialize(Vector3 *aPosition, const char* aTexture)
{
	mVelocity = new Vector3(-1,-1,-1);

	mPosition = aPosition;

	//load individual net parts in order to handle ball hitting different parts (Post vs back of net);

	mNetBack->LoadFromOBJ("Source/Resources/NetFrame2.obj", 1.0f);
	//mNetFrame->LoadFromOBJ("Source/Resources/NetFrame.obj", 1.0f);
	//mNetSide1->LoadFromOBJ("Source/Resources/NetSide1.obj", 1.0f);
	//mNetSide2->LoadFromOBJ("Source/Resources/NetSide2.obj", 1.0f);
	//mNetTop->LoadFromOBJ("Source/Resources/NetTop.obj", 1.0f);
	//m_NetModels.push_back(mNetBack);
	//m_NetModels.push_back(mNetFrame);
	//m_NetModels.push_back(mNetSide1);
	//m_NetModels.push_back(mNetSide2);
	//m_NetModels.push_back(mNetTop);
	mModel = mNetBack;

	mShader = new Shader("Source/Resources/shader.glsl");
	mShader->LoadShaderProgram();
	PullShaderHandles();

	mTextureID = SOIL_load_OGL_texture( "Source/Resources/BlueNet", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 

    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
	glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

void Net::PullShaderHandles()
{
	if(mShader->getProgramHandle())
	{
		mShader->setUniformLocation("u_WorldViewProj");
		mShader->setUniformLocation("u_TexColor");
		mShader->setAttributeLocation("a_Position");
		mShader->setAttributeLocation("a_Tint");
		mShader->setAttributeLocation("a_UVCoord");
	}
}

void Net::RenderSetup(Matrix* pWVP)
{
	glUseProgram(mShader->getProgramHandle());

	GLuint WVPHandle = mShader->getUniform("u_WorldViewProj");
	GLuint TexColHandle =  mShader->getUniform("u_TexColor");

	if(WVPHandle != -1)
	{
		glUniformMatrix4fv(WVPHandle, 1, false, (GLfloat*)&pWVP->m11);
	}

	if(TexColHandle = -1)
	{
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mTextureID);
		glUniform1i( TexColHandle, 0);
	}
}

void Net::Update(double pElapsedTime)
{
	int a = 0;
}

void Net::Draw()
{
	Matrix objpos;
    Matrix worldviewproj;
	Matrix rotation;
    // draw mesh
    {
		objpos.SetIdentity();

		//objpos.Rotate(mRotation->x,mRotation->y,mRotation->z,mRotation->w);

		objpos.SetPosition(*mPosition);
		worldviewproj = objpos;

		Matrix viewProj = *gCamera->GetViewProjection();

		worldviewproj.Multiply( &viewProj );

		RenderSetup(&worldviewproj);
		GLuint PositionHandle = mShader->getAttribute("a_Position");
		GLuint TintHandle = mShader->getAttribute("a_Tint");
		GLuint UVHandle = mShader->getAttribute("a_UVCoord");

		for(int i = 0; i < m_NetModels.size(); i++)
		{
			if(PositionHandle != -1)
			{
				glVertexAttribPointer(PositionHandle, 3, GL_FLOAT, GL_FALSE, sizeof(VertDef), m_NetModels.at(i)->m_pVerts->pos );
				glEnableVertexAttribArray( PositionHandle );
			}

			if(TintHandle != -1)
			{
				glVertexAttribPointer( TintHandle, 4, GL_FLOAT, GL_FALSE, sizeof(VertDef), m_NetModels.at(i)->m_pVerts->col );
				glEnableVertexAttribArray( TintHandle );
			}

			if(UVHandle != -1)
			{
				glVertexAttribPointer( UVHandle, 2, GL_FLOAT, GL_FALSE, sizeof(VertDef), m_NetModels.at(i)->m_pVerts->uv );
				glEnableVertexAttribArray( UVHandle );
			}

			glDrawElements(GL_TRIANGLES, m_NetModels.at(i)->m_NumIndices, GL_UNSIGNED_SHORT, m_NetModels.at(i)->m_pIndices);
		}
	}

	mShader->DisableAttributes();
}
