#include "CommonHeader.h"

TextManager* gTextManager = 0;

TextManager::TextManager()
{
	gTextManager = this;
	m_FontTextureID = -1;
}

TextManager::~TextManager()
{
}

void TextManager::DrawText(char a_StringToDraw[], float a_Height, int a_Justification, Vector3 a_Color, Vector3 a_Position, Camera* a_Camera)
{
	VertDef verts[1000];
    GLushort indicies[1000];

	int indexcount = m_pFont->GenerateVerts( a_StringToDraw, verts, indicies, a_Height, a_Justification, a_Color);

	Matrix objpos;
    Matrix worldviewproj;

    // draw mesh
    
	objpos.SetIdentity();
		
	objpos.SetPosition(a_Position);
	worldviewproj = objpos;

	Matrix viewProj = *a_Camera->GetViewProjection();

	worldviewproj.Multiply( &viewProj );

	RenderSetup(&viewProj);
	GLuint PositionHandle = m_Shader->getAttribute("a_Position");
	GLuint TintHandle = m_Shader->getAttribute("a_Tint");
	GLuint UVHandle = m_Shader->getAttribute("a_UVCoord");

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

	glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_SHORT, indicies);

	m_Shader->DisableAttributes();
}

void TextManager::Initialize(char* a_FontPath, Shader* a_Shader)
{
	m_pFont = new BitmapFont(a_FontPath);
	char tempfontfilename[MAX_PATH];

	sprintf_s( tempfontfilename, "Source/Resources/%s", m_pFont->QueryImageName() );
	m_FontTextureID = SOIL_load_OGL_texture( tempfontfilename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
	m_Shader = a_Shader;
	 
	m_Shader->LoadShaderProgram();
}

void TextManager::PullShaderHandles()
{
	if(m_Shader->getProgramHandle())
	{
		m_Shader->setUniformLocation("u_WorldViewProj");
		m_Shader->setUniformLocation("u_TexColor");
		m_Shader->setAttributeLocation("a_Position");
		m_Shader->setAttributeLocation("a_Tint");
		m_Shader->setAttributeLocation("a_UVCoord");
	}
}

void TextManager::RenderSetup(Matrix* pWVP)
{
	glUseProgram(m_Shader->getProgramHandle());

	GLuint WVPHandle = m_Shader->getUniform("u_WorldViewProj");
	GLuint TexColHandle =  m_Shader->getUniform("u_TexColor");

	if(WVPHandle != -1)
	{
		glUniformMatrix4fv(WVPHandle, 1, false, (GLfloat*)&pWVP->m11);
	}

	if(TexColHandle = -1)
	{
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_FontTextureID);
		glUniform1i( TexColHandle, 0);
	}
}
