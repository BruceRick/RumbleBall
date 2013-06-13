#include "CommonHeader.h"

GameManager* gGameManager = 0;

GameManager::GameManager(OpenGLWindow *pWindow)
{
	


	gGameManager = this;

	m_pFBO = 0;

	m_TextManager = new TextManager();

	mScreenManager = new ScreenManager();
	mCollisionManager = new CollisionManager();
	mGame = new Game();
	mCamera = new Camera();

	
	m_ScreenWidth = pWindow->width();
	m_ScreenHeight = pWindow->height();

	m_TotalTimePassed = 0;

	gCamera->SetPosition(new Vector3(0.0f, 20.0f, -25.0f));
	mShader = 0;

	

	
	float ratio = (float)m_ScreenWidth/m_ScreenHeight;
	gCamera->GetPerspective()->SetPerspective( 90, ratio, 1.0f, 100000);
	gCamera->GetOrth()->SetOrtho(0, (float)m_ScreenWidth, 0, (float)m_ScreenHeight, 0, 1 );

}

GameManager::~GameManager()
{

}

void GameManager::OneTimeInit()
{
	
	

	glViewport( 0, 0, m_ScreenWidth, m_ScreenHeight );

	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_pFBO = new FrameBuffer();
    m_pFBO->Setup(1024, 1024);

	mShader = new Shader("Source/Resources/shader.glsl");
	mShader->LoadShaderProgram();
	if(mShader->getProgramHandle())
	{
		mShader->setUniformLocation("u_WorldViewProj");
		mShader->setUniformLocation("u_TexColor");
		mShader->setAttributeLocation("a_Position");
		mShader->setAttributeLocation("a_Tint");
		mShader->setAttributeLocation("a_UVCoord");

	}

	gCamera->Position()->Set(0, 35.0f,100.0f);
	
	gCamera->GetViewProjection()->SetLookAtRH( *gCamera->Position(), Vector3(0,1,0), Vector3(0,0,0));

	m_TextManager->Initialize("Source/Resources/brucefont.fnt", mShader);


	mGame->Initialize();
	mScreenManager->AddScreen((Screen*)mGame);

}

void GameManager::Update(double pElapsedTime)
{

	mScreenManager->Update(pElapsedTime);

}

void GameManager::Draw()
{

	//MODELS

	Matrix objpos;
    Matrix worldviewproj;

    // clear the screen.
    glClearColor( 0.0f, 0.0f, 0.5f, 0.0f );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // setup our camera
	//gCamera->GetViewProjection()->SetLookAtRH( *gCamera->Position(), Vector3(0,1,0), Vector3(0,0,0));

	gCamera->GetViewProjection()->Multiply(gCamera->GetPerspective());
    
	glBindFramebuffer( GL_FRAMEBUFFER, m_pFBO->m_FBOHandle );
    glViewport( 0, 0, m_pFBO->m_Width, m_pFBO->m_Height );

	

	

    // draw mesh
    {

		glClearColor( 0.23f, 0.34f, 0.74f, 0.0f );
		glClearDepth( 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        objpos.SetIdentity();
        objpos.SetPosition( 0,0,0 );

		worldviewproj = objpos;
		worldviewproj.Multiply( gCamera->GetViewProjection() );

		mScreenManager->Draw();
        
    }


	// deactivate the framebuffer.
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glViewport( 0, 0, m_ScreenWidth, m_ScreenHeight );

    glBindTexture( GL_TEXTURE_2D, m_pFBO->m_TextureHandle );


	DrawFrameBuffer();
}

void GameManager::DrawFrameBuffer()
{

	float scrw = (float)m_ScreenWidth;
    float scrh = (float)m_ScreenHeight;
    VertDef verts[] = {     0,    0, 0,   1,1,1,1,   0, 0,  0,0,0, // bl
                             scrw,    0, 0,   1,1,1,1,   1, 0,  0,0,0, // br
                                0, scrh, 0,   1,1,1,1,   0, 1,  0,0,0, // tl
                             scrw, scrh, 0,   1,1,1,1,   1, 1,  0,0,0, }; // tr

    unsigned short indices[] = { 0, 3, 2, 0, 1, 3 };


	glUseProgram(mShader->getProgramHandle());

	GLuint WVPHandle = mShader->getUniform("u_WorldViewProj");
	GLuint TexColHandle =  mShader->getUniform("u_TexColor");

	if(WVPHandle != -1)
	{
		Matrix* orthoMat = gCamera->GetOrth();
		glUniformMatrix4fv(WVPHandle, 1, false, (GLfloat*)&orthoMat->m11);
	}

	if(TexColHandle = -1)
	{
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_pFBO->m_TextureHandle);
		glUniform1i( TexColHandle, 0);
	}


	GLuint PositionHandle = mShader->getAttribute("a_Position");
	GLuint TintHandle = mShader->getAttribute("a_Tint");
	GLuint UVHandle = mShader->getAttribute("a_UVCoord");

	if(PositionHandle != -1)
	{
		glVertexAttribPointer(PositionHandle, 3, GL_FLOAT, GL_FALSE, sizeof(VertDef), &verts->pos );

		glEnableVertexAttribArray( PositionHandle );
	}

	if(TintHandle != -1)
	{
		glVertexAttribPointer( TintHandle, 4, GL_FLOAT, GL_FALSE, sizeof(VertDef), &verts->col );
        glEnableVertexAttribArray( TintHandle );
	}

	if(UVHandle != -1)
	{
		glVertexAttribPointer( UVHandle, 2, GL_FLOAT, GL_FALSE, sizeof(VertDef), &verts->uv );
        glEnableVertexAttribArray( UVHandle );
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	mShader->DisableAttributes();

}


ScreenManager GameManager::GetScreenManager()
{

	return *mScreenManager;
}

