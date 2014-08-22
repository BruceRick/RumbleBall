#ifndef __OpenGLWindow_H__
#define __OpenGLWindow_H__

class OpenGLWindow;
class GameManager;

extern OpenGLWindow* gWindow;

class OpenGLWindow
{
public:
	OpenGLWindow( const char* pWindowTitle, int width, int height);
	~OpenGLWindow();

	//void ProcessInput();
	void Initialize();
	
	void GLFWCALL WindowResize(int pWidth, int pHeight);
	void Update();
	void Draw();

	int width();
	int height();

public:
	int mScreenWidth;
	int mScreenHeight;
	const char* mWindowTitle;
	double mPreviousTime;
	GameManager* mGameManager;
};

#endif
