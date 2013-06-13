#ifndef __FrameBuffer_H__
#define __FrameBuffer_H__

class FrameBuffer
{
public:
	GLuint m_FBOHandle;

	GLuint m_TextureHandle;
	GLuint m_DepthBufferHandle;

	int m_Width;
	int m_Height;

public:
	FrameBuffer();
	~FrameBuffer();

	void Setup(int width, int height);
};

#endif //__FrameBuffer_H__
