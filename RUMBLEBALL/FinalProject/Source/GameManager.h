#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

class GameManager;
class ScreenManager;
class Game;
class FrameBuffer;
class MeshObject;
class ShaderBase;
class Shader;

extern GameManager* gGameManager;

class GameManager
{
public:
	GameManager(OpenGLWindow* pWindow);
	~GameManager();
	void Update(double pElapsedTime);
	void Draw();
	ScreenManager GetScreenManager();
	void OneTimeInit();
	void DrawFrameBuffer();

private:
	FrameBuffer* m_pFBO;
	Shader *aShader;
	ScreenManager* mScreenManager; 
	CollisionManager* mCollisionManager;
	TextManager* m_TextManager;
	Game* mGame;

	int m_ScreenWidth;
	int m_ScreenHeight;
	float m_TotalTimePassed;
	
	//Matrix m_matViewProj;
	Camera* mCamera;
	//ShaderBase* m_pShader;
	Shader* mShader;
};

#endif
