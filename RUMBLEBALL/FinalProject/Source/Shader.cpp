#include "CommonHeader.h"

Shader::Shader()
{
}

Shader::Shader(const char* pShaderFileName)
{
	m_VertexShaderHandle = 0;
    m_FragmentShaderHandle = 0;
    m_ProgramHandle = 0;
	m_ShaderFileName = pShaderFileName;
}

Shader::~Shader()
{
    glDetachShader( m_ProgramHandle, m_VertexShaderHandle );
    glDetachShader( m_ProgramHandle, m_FragmentShaderHandle );    
    glDeleteShader( m_VertexShaderHandle );
    glDeleteShader( m_FragmentShaderHandle );
    glDeleteProgram( m_ProgramHandle );
}

void Shader::LoadShaderProgram()
{
	FILE* file;
    errno_t err = fopen_s( &file, m_ShaderFileName, "rb" );

    char* buffer;
    int length;

    if( file )
    {
        fseek( file, 0, SEEK_END );
        long size = ftell( file );
        rewind( file );

        buffer = new char[size];
        fread( buffer, size, 1, file );

        length = size;
        
        fclose( file );
    }

	m_ProgramHandle = CreateProgram( length, buffer, length, buffer, &m_VertexShaderHandle, &m_FragmentShaderHandle );

	delete buffer;
}

GLuint Shader::getProgramHandle()
{
	return m_ProgramHandle;
}

//std::vector<GLuint> Shader::getAttributes()
//{
//	return m_Attributes;
//}
//
//std::vector<GLuint> Shader::getUniforms()
//{
//	return m_Uniforms;
//}

GLuint Shader::LoadShader(GLenum shaderType, const char* pPreSource, int presourcelen, const char* pSource, int sourcelen)
{
	GLuint shaderid = glCreateShader(shaderType);
    if( shaderid )
    {
        const char* sources[2] = { pPreSource, pSource };
        int lengths[2] = { presourcelen, sourcelen };
        glShaderSource( shaderid, 2, sources, lengths );
        glCompileShader( shaderid );
        GLint compiled = 0;
        glGetShaderiv( shaderid, GL_COMPILE_STATUS, &compiled );
        if( !compiled )
        {
            GLint infoLen = 0;
            glGetShaderiv( shaderid, GL_INFO_LOG_LENGTH, &infoLen );
            if( infoLen )
            {
                char* buf = (char*)malloc(infoLen);
                if( buf )
                {
                    glGetShaderInfoLog( shaderid, infoLen, NULL, buf );
					ErrorLog( "Could not compile shader %d:\n%s\n", shaderType, buf );
                    ErrorLog( "Shader Lengths: Pre-source: %d  source len: %d\n", presourcelen, sourcelen );
                    ErrorLog( "Shader Pre-source: \n%s\n", pPreSource );
                    ErrorLog( "Shader source: \n%s\n", pSource );
                    free( buf );

            		assert( false );
                }
                glDeleteShader( shaderid );
                shaderid = 0;
            }
        }
    }

    return shaderid;
}

GLuint Shader::CreateProgram(int vslen, const char* pVertexSource, int fslen, const char* pFragmentSource, GLuint* vsid, GLuint* fsid, int prevslen, const char* pPreVertexSource, int prefslen, const char* pPreFragmentSource)
{
	// Try to load the vertex shader.
    *vsid = LoadShader( GL_VERTEX_SHADER, pPreVertexSource, prevslen, pVertexSource, vslen );
    if( *vsid == 0 )
    {
        return 0;
    }

    // Try to load the fragment shader.
    *fsid = LoadShader( GL_FRAGMENT_SHADER, pPreFragmentSource, prefslen, pFragmentSource, fslen );
    if( *fsid == 0 )
    {
        glDeleteShader( *vsid );
        *vsid = 0;
        return 0;
    }

    // Try to create a program.
    GLuint programid = glCreateProgram();
    if( programid == 0 )
    {
        glDeleteShader( *vsid );
        glDeleteShader( *fsid );
        *vsid = 0;
        *fsid = 0;
        return 0;
    }

    // Attach the shaders to the program and attempt to link them.
    glAttachShader( programid, *vsid );

    glAttachShader( programid, *fsid );

    glLinkProgram( programid );
    GLint linkStatus = GL_FALSE;
    glGetProgramiv( programid, GL_LINK_STATUS, &linkStatus );

    if( linkStatus != GL_TRUE )
    {
        GLint bufLength = 0;
        glGetProgramiv( programid, GL_INFO_LOG_LENGTH, &bufLength );
        if( bufLength )
        {
            char* buf = (char*)malloc( bufLength );
            if( buf )
            {
                glGetProgramInfoLog( programid, bufLength, 0, buf );
                free(buf);
            }
        }
        glDeleteProgram( programid );
        glDeleteShader( *vsid );
        glDeleteShader( *fsid );
        programid = 0;
        *vsid = 0;
        *fsid = 0;
        return 0;
	}

	return programid;
}

void Shader::setAttributeLocation( const char* pAttributeName)
{
	ShaderParameter Attribute;
	Attribute.handleName = pAttributeName;
	Attribute.ParameterHandle = -1;
	Attribute.ParameterHandle = glGetAttribLocation( m_ProgramHandle, pAttributeName);

	m_Attributes.push_back(Attribute);

	if(Attribute.ParameterHandle == -1 )
		ErrorLog( "%s = -1\n", Attribute);
}

void Shader::setUniformLocation( const char* pUniformName)
{
	ShaderParameter Uniform;
	Uniform.handleName = pUniformName;
	Uniform.ParameterHandle = -1;
	Uniform.ParameterHandle = glGetUniformLocation( m_ProgramHandle, pUniformName);

	//GLint Uniform = glGetUniformLocation( m_ProgramHandle, uniformName);

	m_Uniforms.push_back(Uniform);

	if(Uniform.ParameterHandle == -1 )
		ErrorLog( "%s = -1\n", Uniform);
}

void Shader::DisableAttributes()
{
	for(int i = 0; i < m_Attributes.size() ; i++)
	{
		glDisableVertexAttribArray(m_Attributes.at(i).ParameterHandle);
	}
}

GLuint Shader::getUniform(const char* pUniformName)
{
	for(int i = 0; i < m_Uniforms.size(); i++)
	{
		if(m_Uniforms.at(i).handleName == pUniformName)
		{
			return m_Uniforms.at(i).ParameterHandle;
		}
	}
	return -1;
	ErrorLog("No shader uniform named ", pUniformName);
}

GLuint Shader::getAttribute(const char* pAttributeName)
{
	for(int i = 0; i < m_Attributes.size(); i++)
	{
		if(m_Attributes.at(i).handleName == pAttributeName)
		{
			return m_Attributes.at(i).ParameterHandle;
		}
	}
	return -1;
	ErrorLog("No shader attribute named ", pAttributeName);
}
