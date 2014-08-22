#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__

class TextManager;

extern TextManager* gTextManager;

class TextManager
{
public:
	TextManager();
	~TextManager();
	void Initialize(char* a_FontPath, Shader* a_Shader);
	void DrawText(char a_StringToDraw[], float a_Height, int a_Justification, Vector3 a_Color, Vector3 a_Position, Camera* a_Camera);
	void PullShaderHandles();
	void RenderSetup(Matrix* pWVP);

private:
	BitmapFont* m_pFont;
	GLuint m_FontTextureID;
	Shader* m_Shader;
};

#endif
