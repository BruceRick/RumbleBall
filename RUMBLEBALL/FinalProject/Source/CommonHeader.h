#ifndef __CommonHeader_H__
#define __CommonHeader_H__


#define PI 3.1415926535897932384626433832795f

#define SAFE_DELETE(x) { if(x) delete x; x=0; }

#define Justify_Top         0x01
#define Justify_Bottom      0x02
#define Justify_CenterY     0x04
#define Justify_Left        0x08
#define Justify_Right       0x10
#define Justify_CenterX     0x20


#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <time.h>
#include <conio.h>
#include <process.h>
#include <cstdlib>
#include <iostream>
#include <sstream>






struct VertDef
{
	float pos[3];
	float col[4];
	float uv[2];
	float normal[3];
};


struct PlayerKeys
{
	int Throttle;
	int RotateLeft;
	int RotateRight;


};

struct PlayingFieldBounds
{

	float MaxX;
	float MaxZ;
	float MinX;
	float MinZ;

};


#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/glfw.h"

#undef glGenFramebuffers
#define glGenFramebuffers glGenFramebuffersEXT
#undef glBindFramebuffer
#define glBindFramebuffer glBindFramebufferEXT
#undef glFramebufferTexture2D
#define glFramebufferTexture2D glFramebufferTexture2DEXT
#undef glCheckFramebufferStatus
#define glCheckFramebufferStatus glCheckFramebufferStatusEXT
#undef glDeleteFramebuffers
#define glDeleteFramebuffers glDeleteFramebuffersEXT

#include "Utility/ErrorLog.h"
#include "Utility/Vector.h"
#include "Utility/Matrix.h"
#include "Utility/LoadFile.h"
#include "Utility/objloader/objLoader.h"
#include "Utility/SOIL/Soil.h"



struct Material
{
	Vector4 MaterialDiffuse;
	Vector4 MaterialSpecular;
	Vector4 MaterialAmbient;

};

struct Light
{
	Vector3 LightPosition;
	Vector4 LightDiffuse;
	Vector4 LightSpecular;
	Vector4 LightAmbient;
	float SpecularPower;


};


//TEXTURE LOADER

#include <stdlib.h>



#include "OpenGLWindow.h"
#include "ScreenManager.h"
#include "Screen.h"
#include "MeshObject.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputManager.h"
#include "Entity.h"
#include "Player.h"
#include "Ball.h"
#include "Net.h"
#include "PhysicsManager.h"
#include "Game.h"
#include "CollisionManager.h"
#include "BitmapFont.h"
#include "TextManager.h"



#include "GameManager.h"










#endif





