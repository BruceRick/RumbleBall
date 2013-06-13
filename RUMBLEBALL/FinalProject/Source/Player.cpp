#include "CommonHeader.h"


Player::Player(PlayerKeys* pPlayerKeys, const char* pCarTexture, Vector3* pPosition)
{
	mInputState = new InputManager(pPlayerKeys);
	mPosition = pPosition;
	mRotation = new Vector4(0,0,1,0);
	mRotationRate = 0;
	mCarTexture = pCarTexture;
	
}

Player::~Player()
{
	TerminateThread(mThrottleThread,0);
	TerminateThread(mRotateRightThread,0);
	TerminateThread(mRotateLeftThread,0);
}

DWORD WINAPI Throttle(LPVOID lpParam)
{




	Player* player = (Player*)lpParam;

	while(true)
	{
		WaitForSingleObject(player->GetInputState()->getThrottleEvent(),INFINITE);
		

		Vector3 velocity = *player->GetVelocity();

		// when the user wants to move forward:
		double angle = player->mRotationRate * PI / 180;

		

		velocity.x += cos( angle ) * 20 *player->mTimeElapsed;
		velocity.z += sin( angle ) * 20 *player->mTimeElapsed;

		// here, to stop the ship from exceeding a maximum velocity, you can calculate the overall
		//   speed and scale down the velocity appropriately

		//double totalvelocity = sqrt( (velocity.x*velocity.x) + (velocity.y*velocity.y) );

		//if( totalvelocity > 0.5 )
		//{
		//	 // ship is moving too fast.  Scale the velocity down
		//	 velocity.x *= 0.5 / totalvelocity;
		//	 velocity.z *= 0.5 / totalvelocity;
		//}

		
		//position.z += 0.7f;
		player->SetVelocity(&velocity);
		

	}
}

DWORD WINAPI RotateLeft(LPVOID lpParam)
{
	Player* player = (Player*)lpParam;

	while(true)
	{
		WaitForSingleObject(player->GetInputState()->getRotateLeftEvent(),INFINITE);
		player->mRotationRate -= 75.0f *player->mTimeElapsed;
		//player->GetRotation()->Set(player->GetRotation()->x - 0.1f,player->GetRotation()->y,player->GetRotation()->z,player->GetRotation()->w);
	}
}


DWORD WINAPI RotateRight(LPVOID lpParam)
{
	Player* player = (Player*)lpParam;

	while(true)
	{
		WaitForSingleObject(player->GetInputState()->getRotateRightEvent(),INFINITE);
		player->mRotationRate += 75.0f *player->mTimeElapsed;
		//player->GetRotation()->Set(player->GetRotation()->x + 0.1f,player->GetRotation()->y,player->GetRotation()->z,player->GetRotation()->w);
	}
}

void Player::Initialize()
{

	//mPosition = new Vector3(0, 60.0f, 0.0f);
	mVelocity = new Vector3(0,0,0);
	mInputState->Initialize();

	mShader = new Shader("Source/Resources/shader.glsl");
	mShader->LoadShaderProgram();
	PullShaderHandles();

	mTextureID = SOIL_load_OGL_texture( mCarTexture, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
	glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	mModel = new MeshObject();
	mModel->LoadFromOBJ("Source/Resources/car1.obj",5.0f);
	mThrottleThread = CreateThread(NULL, 0, Throttle, this, 0, NULL);
	mRotateLeftThread = CreateThread(NULL, 0, RotateRight, this, 0, NULL);
	mRotateRightThread = CreateThread(NULL, 0, RotateLeft, this, 0, NULL);
	

}

void Player::PullShaderHandles()
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

void Player::Update(double pElapsedTime)
{
	
	mTimeElapsed = pElapsedTime;
	mInputState->Update();
	std::stringstream ss;
	ss << mVelocity->x;

	std::string velocity = ss.str();

	//ErrorLog(velocity.c_str());
	//ErrorLog("\n");

}

void Player::RenderSetup(Matrix* pWVP)
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

void Player::Draw()
{

	Matrix objpos;
    Matrix worldviewproj;
	Matrix rotation;
    // draw mesh
    {
		objpos.SetIdentity();
		
		

		//while( mRotationRate < 0 ) mRotationRate += 360;
		//while( mRotationRate > 360 ) mRotationRate -= 360;

		mRotation->Set(mRotationRate,mRotation->y, mRotation->z, mRotation->w);

		objpos.Rotate(mRotation->x + 270,mRotation->y,mRotation->z,mRotation->w);
		

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


InputManager* Player::GetInputState()
{
	return mInputState;
}

Vector3* Player::GetPosition()
{
	return mPosition;
}

Vector3* Player::GetVelocity()
{
	return mVelocity;
}

Vector4* Player::GetRotation()
{
	return mRotation;
}

void Player::SetPosition(Vector3* pPosition)
{
	mPosition = pPosition;
}

void Player::SetVelocity(Vector3* pVelocity)
{
	mVelocity = pVelocity;
}



