#include "CommonHeader.h"


Entity::Entity()
{
	

}


Entity::~Entity()
{


}


void Entity::Initialize(MeshObject* pModel, GLuint pTextureID, Shader* pShader)
{
	mVelocity = new Vector3(-1,-1,-1);
	mPosition = new Vector3(0,0,0);
	mModel = pModel;
	mTextureID = pTextureID;
	mShader = pShader;
	
	mMaterial = new Material;
	

	mShader->LoadShaderProgram();
	PullShaderHandles();


	glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	


}

void Entity::Update(double pElapsedTime)
{

}


void Entity::Draw()
{
	Matrix objpos;
    Matrix worldviewproj;

    // draw mesh
    {
		objpos.SetIdentity();
		
		objpos.SetPosition(*mPosition);
		worldviewproj = objpos;

		Matrix viewProj = *gCamera->GetViewProjection();

		worldviewproj.Multiply( &viewProj );


		RenderSetup(&viewProj);
		GLuint PositionHandle = mShader->getAttribute("a_Position");
		GLuint TintHandle = mShader->getAttribute("a_Tint");
		GLuint UVHandle = mShader->getAttribute("a_UVCoord");

		if(PositionHandle != -1)
		{
			glVertexAttribPointer(PositionHandle, 3, GL_FLOAT, GL_FALSE, sizeof(VertDef), &mModel->m_pVerts->pos );

			glEnableVertexAttribArray( PositionHandle );
		}

		if(TintHandle != -1)
		{
			glVertexAttribPointer( TintHandle, 4, GL_FLOAT, GL_FALSE, sizeof(VertDef), &mModel->m_pVerts->col );
			glEnableVertexAttribArray( TintHandle );
		}

		if(UVHandle != -1)
		{
			glVertexAttribPointer( UVHandle, 2, GL_FLOAT, GL_FALSE, sizeof(VertDef), &mModel->m_pVerts->uv );
			glEnableVertexAttribArray( UVHandle );
		}

		glDrawElements(GL_TRIANGLES, mModel->m_NumIndices, GL_UNSIGNED_SHORT, mModel->m_pIndices);


	}


	mShader->DisableAttributes();

}

void Entity::RenderSetup(Matrix* pWVP)
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


void Entity::PullShaderHandles()
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


MeshObject* Entity::GetModel()
{
	return mModel;
}

