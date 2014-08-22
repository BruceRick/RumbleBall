#include "CommonHeader.h"

OpenGLWindow* gWindow = 0;

OpenGLWindow::OpenGLWindow( const char* p_WindowTitle, int width, int height)
{
	gWindow = this;
	mWindowTitle = p_WindowTitle;
	
	mScreenWidth = width;
	mScreenHeight = height;
	
	mGameManager = new GameManager(gWindow);
}

void OpenGLWindow::Initialize()
{
	glfwOpenWindow( mScreenWidth, mScreenHeight, 8,8,8,8,31,1, GLFW_WINDOW );
	WindowResize(mScreenWidth, mScreenHeight);	
	mPreviousTime = glfwGetTime();
}

OpenGLWindow::~OpenGLWindow()
{
	glfwTerminate();
}

int OpenGLWindow::width()
{
	return mScreenWidth;
}

int OpenGLWindow::height()
{
	return mScreenHeight;
}

void OpenGLWindow::Update()
{
	double currentTime = glfwGetTime();
    double elapsedTime = currentTime - mPreviousTime;
    mPreviousTime = currentTime;

	if( glfwGetWindowParam( GLFW_OPENED ) )
    {
		glfwSwapBuffers();
		mGameManager->Update(elapsedTime);
	}
}

void OpenGLWindow::Draw()
{
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mGameManager->Draw();
}

void GLFWCALL OpenGLWindow::WindowResize(int width, int height)
{
	glfwSetWindowTitle( mWindowTitle );
	glViewport( 0, 0, width, height );
    mScreenWidth = width;
    mScreenHeight = height;
}
