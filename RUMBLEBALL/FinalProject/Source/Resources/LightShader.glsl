#ifdef VertexShader

uniform mat4 u_WorldViewProj;
uniform mat4 u_World;
uniform vec3 u_CamPos;
attribute vec4 a_Position;
attribute vec4 a_Tint;
attribute vec2 a_UVCoord;
attribute vec3 a_Normal;

uniform vec4 u_MatAmb;
uniform vec4 u_MatDif;
uniform vec4 u_MatSpec;

varying vec4 v_Color;
varying vec2 v_UVCoord;

struct LightDef
{
	vec3 pos;
	vec4 col_Amb;
	vec4 col_Dif;
	vec4 col_Spec;
	float SpecPower;
};

uniform LightDef u_Lights[3];

vec4 DirLight(int i, vec3 norm)
{
	vec3 lightdir = u_Lights[i].pos;
	float NdotL = max(0, dot(normalize(lightdir), norm));
	vec4 diffuse = NdotL * u_Lights[i].col_Dif * u_MatDif;
	vec4 ambient = u_Lights[i].col_Amb * u_MatAmb;
	vec3 halfVec = normalize( lightdir + u_CamPos );
	float NdotH = max( 0.0, dot( norm, halfVec ) );
	vec4 specular = pow(NdotH, u_Lights[i].SpecPower) * u_Lights[i].col_Spec * u_MatSpec;

	return ambient + diffuse + specular;
}

vec4 PointLight(int i, vec3 norm, vec4 vertWorldPos)
{
	vec4 lightdir = vec4(u_Lights[i].pos, 1) - vertWorldPos;
	float NdotL = max(0, dot(normalize(lightdir), norm));
	vec4 diffuse = NdotL * u_Lights[i].col_Dif * u_MatDif;
	vec4 ambient = u_Lights[i].col_Amb * u_MatAmb;
	vec3 halfVec = normalize( lightdir + u_CamPos );
	float NdotH = max( 0.0, dot( norm, halfVec ) );
	vec4 specular = pow(NdotH, u_Lights[i].SpecPower) * u_Lights[i].col_Spec * u_MatSpec;
	float dist = length( lightdir );
	vec3 light_attenuation = vec3(0.1, 0.1, 0.1);
	float attenuation = 1 / (light_attenuation.x + light_attenuation.y * dist + light_attenuation.z * dist * dist);

	return ( ambient + diffuse + specular) * attenuation;
}

void main()
{
	gl_Position = u_WorldViewProj * a_Position;

	vec3 wNormal = normalize(mat3(u_World) * a_Normal);
	vec4 vertWorldPos = u_World * a_Position;	

	v_Color = DirLight(0, wNormal)
			+ DirLight(1, wNormal)
			+ PointLight(2, wNormal, vertWorldPos);
			+ PointLight(3, wNormal, vertWorldPos);

	v_UVCoord = a_UVCoord;
}

#endif

#ifdef FragmentShader

uniform sampler2D u_TexColor;
varying vec4 v_Color;
varying vec2 v_UVCoord;

void main()
{	
    gl_FragColor = v_Color;// * texture2D( u_TexColor, v_UVCoord );// * v_Color;
}

#endif