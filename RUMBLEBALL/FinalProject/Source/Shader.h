#ifndef __Shader_H__
#define __Shader_H__

#define VERTEXPREDEFINES "#define VertexShader 1\n"
#define FRAGMENTPREDEFINES "#define FragmentShader 1\n"
#define CheckGlError(op) CheckGlErrorFunc(op, __FILE__, __LINE__)

struct ShaderParameter
{
	const char* handleName;
	GLint ParameterHandle;
};

class Shader
{
public:
	Shader();
	Shader( const char* pShaderFileName);
	~Shader();

	GLuint LoadShader(GLenum shaderType, const char* pPreSource, int presourcelen, const char* pSource, int sourcelen);
	GLuint CreateProgram(int vslen, const char* pVertexSource, int fslen, const char* pFragmentSource, GLuint* vsid, GLuint* fsid, int prevslen = strlen(VERTEXPREDEFINES), const char* pPreVertexSource = VERTEXPREDEFINES, int prefslen = strlen(FRAGMENTPREDEFINES), const char* pPreFragmentSource = FRAGMENTPREDEFINES);
	GLuint getProgramHandle();
	//std::vector<ShaderParameter> getAttributes();
	//std::vector<ShaderParameter> getUniforms();

	void setAttributeLocation( const char* pAttributeName);
	void setUniformLocation( const char* pUniformName);
	GLuint getUniform(const char* pUniformName);
	GLuint getAttribute(const char* pAttributeName);
	void DisableAttributes();
	void LoadShaderProgram();

private:
	GLuint m_VertexShaderHandle;
    GLuint m_FragmentShaderHandle;
    GLuint m_ProgramHandle;

	std::vector<ShaderParameter> m_Attributes;
	std::vector<ShaderParameter> m_Uniforms;
	const char* m_ShaderFileName;
};

#endif 
