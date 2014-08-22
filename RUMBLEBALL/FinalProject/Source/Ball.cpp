#include "CommonHeader.h"

Ball::Ball()
{
	mRotation = new Vector4(0,0,1,0);
	mRotationRate = new Vector3(0,0,0);
}

Ball::~Ball()
{
}

void Ball::Initialize()
{
	mPosition = new Vector3(0, 100.0f, 0.0f);
	mVelocity = new Vector3(0,0,0);

	mShader = new Shader("Source/Resources/shader.glsl");
	mShader->LoadShaderProgram();
	PullShaderHandles();

	mTextureID = SOIL_load_OGL_texture( "Source/Resources/soccerTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 

        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
	glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	mModel = new MeshObject();
	mModel->LoadFromOBJ("Source/Resources/Soccerball.obj",10.0f);
}

void Ball::PullShaderHandles()
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

void Ball::Update(double pElapsedTime)
{
	mTimeElapsed = pElapsedTime;

	mRotationRate->z += mVelocity->z;
	mRotationRate->x += mVelocity->x;
}

void Ball::RenderSetup(Matrix* pWVP)
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

void Ball::Draw()
{
	Matrix objpos;
    Matrix worldviewproj;
	Matrix rotation;
    // draw mesh
    {
		objpos.SetIdentity();

		while( mRotationRate->x < 0 ) mRotationRate->x += 360;
		while( mRotationRate->x > 360 ) mRotationRate->x -= 360;

		mRotation->Set(mRotationRate->z,0, mRotationRate->x, 0);

		double angle = (mRotationRate->x + mRotationRate->z) * PI / 180;

		//objpos.Rotate(angle,0,0,0);

		objpos.Rotate( mRotation->x,mRotation->x,0,0);
		objpos.Rotate( mRotation->z,0,0,mRotation->z);

		mPosition->x += mVelocity->x;
		mPosition->z += mVelocity->z;

		objpos.SetPosition(*mPosition);
		worldviewproj = objpos;

		Matrix viewProj = *gCamera->GetViewProjection();

		worldviewproj.Multiply( &viewProj );

		RenderSetup(&worldviewproj);
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

Vector3* Ball::GetPosition()
{
	return mPosition;
}

Vector3* Ball::GetVelocity()
{
	return mVelocity;
}

Vector4* Ball::GetRotation()
{
	return mRotation;
}

void Ball::SetPosition(Vector3* pPosition)
{
	mPosition = pPosition;
}

void Ball::SetVelocity(Vector3* pVelocity)
{
	mVelocity = pVelocity;
}
