#ifdef VertexShader

uniform mat4 u_WorldViewProj;
attribute vec4 a_Position;
attribute vec4 a_Tint;
attribute vec2 a_UVCoord;
varying vec4 v_Color;
varying vec2 v_UVCoord;

void main()
{
    gl_Position = u_WorldViewProj * a_Position;
	v_Color = a_Tint;
	v_UVCoord = a_UVCoord;
	//v_Normal = a_Normal;
}

#endif

#ifdef FragmentShader

uniform sampler2D u_TexColor;
varying vec4 v_Color;
varying vec2 v_UVCoord;

void main()
{
    gl_FragColor = texture2D( u_TexColor, v_UVCoord );// * v_Color;
}

#endif

