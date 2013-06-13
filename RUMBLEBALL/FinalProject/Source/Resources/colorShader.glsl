#ifdef VertexShader

uniform mat4 u_WorldViewProj;
attribute vec4 a_Position;
attribute vec4 a_Tint;
varying vec4 v_Color;

void main()
{
	
	

    gl_Position = u_WorldViewProj * a_Position;
	
}



#endif

#ifdef FragmentShader

varying vec4 v_Color;


void main()
{
    gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}

#endif



