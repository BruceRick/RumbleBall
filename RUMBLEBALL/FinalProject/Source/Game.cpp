#include "CommonHeader.h"

Game::Game()
{
	PlayerKeys* Player1 = new PlayerKeys;
	Player1->Throttle = 'W';
	Player1->RotateLeft = 'A';
	Player1->RotateRight = 'D';
	PlayerKeys* Player2 = new PlayerKeys;
	Player2->Throttle =  'I';
	Player2->RotateLeft = 'J';
	Player2->RotateRight = 'L';
	mPlayer = new Player(Player1, "Source/Resources/bluetexture.png", new Vector3(0, 60.0f, -2300.0f));
	mPlayer2 = new Player(Player2, "Source/Resources/redtexture.png",new Vector3(0, 60.0f, 2300.0f));
	mBall = new Ball();
	mStadium = new Entity();
	mSkySphere = new Entity();
	mTeam1Net = new Net();
	mTeam2Net = new Net();

	mEntityList.push_back((Entity*)mPlayer);
	mEntityList.push_back((Entity*)mPlayer2);
	mEntityList.push_back((Entity*)mBall);
	mEntityList.push_back((Entity*)mTeam1Net);
	mEntityList.push_back((Entity*)mTeam2Net);
	mEntityList.push_back(mStadium);
	mEntityList.push_back(mSkySphere);
}

Game::~Game()
{
}

void Game::Initialize()
{
	Material* RedMaterial;

	mCameraDistance = 300.0f;
	mPlayer->Initialize();
	mPlayer2->Initialize();
	mBall->Initialize();
	MeshObject* Stadium = new MeshObject();
	MeshObject* Sphere = new MeshObject();
	
	Stadium->LoadFromOBJ("Source/Resources/SoccerStadium2.obj", 0.5f);
	Sphere->LoadFromOBJ("Source/Resources/skySphere.obj", 0.5f);
	
	mStadium->Initialize(Stadium, SOIL_load_OGL_texture( "Source/Resources/StadiumTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y ), new Shader("Source/Resources/shader.glsl") );
	mSkySphere->Initialize(Sphere, SOIL_load_OGL_texture( "Source/Resources/crate2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y ), new Shader("Source/Resources/shader.glsl"));
	
	PlayingFieldBounds playingField;
	playingField.MaxX = 2220;
	playingField.MaxZ = 3420;
	playingField.MinX = -2220;
	playingField.MinZ = -3420;

	mTeam1Net->Initialize(new Vector3( 0, 60.0f, 0), "Source/Resources/BlueNet");
	mTeam2Net->Initialize(new Vector3( 0, 60.0f, 0), "Source/Resources/RedNet");
	

	mPhysicsManager = new PhysicsManager(mEntityList, playingField);
}

float fLerp(float start, float end, float percent)
{
	return (start + percent*(end - start));
}

void Game::Update(double pElapsedTime)
{
	for(int i = 0; i < mEntityList.size(); i++)
	{
		mEntityList.at(i)->Update(pElapsedTime);
	}

	//CameraAbove
	//gCamera->Position()->Set(0, 2500.0f,0);
	//gCamera->GetViewProjection()->SetLookAtRH( *gCamera->Position(), Vector3(1,0,0), Vector3(0,0,0));

	//CameraFollowTarget
	double angle = mPlayer->mRotationRate * PI / 180;
		
	float ForwardZ = -sin( angle ) * mCameraDistance;
	float ForwardX = -cos( angle ) * mCameraDistance;

	//CameraFollow

	gCamera->Position()->Set(
		fLerp(gCamera->Position()->x,mPlayer->GetPosition()->x + ForwardX,pElapsedTime * 5.0f),
		fLerp(gCamera->Position()->y,mPlayer->GetPosition()->y + 100.0f,pElapsedTime * 5.0f),
		fLerp(gCamera->Position()->z,mPlayer->GetPosition()->z + ForwardZ,pElapsedTime * 5.0f));

	//gCamera->Position()->Set(mPlayer->GetPosition()->x + ForwardX,mPlayer->GetPosition()->y + 100.0f,mPlayer->GetPosition()->z + ForwardZ);
	gCamera->GetViewProjection()->SetLookAtRH( *gCamera->Position(), Vector3(0,1,0), *mPlayer->GetPosition());
	//gCamera->GetViewProjection()->SetLookAtRH( m, Vector3(0,1,0), *mPlayer->GetPosition());

	mPhysicsManager->ApplyPhysics(pElapsedTime);

	if(gCollisionManager->isCollided((Entity*)mPlayer,(Entity*)mPlayer2))
	{
		mPlayer->GetVelocity()->Set(mPlayer->GetVelocity()->x *-1,mPlayer->GetVelocity()->y,mPlayer->GetVelocity()->z *-1);
	}
	
	if(gCollisionManager->isCollided((Entity*)mPlayer2,(Entity*)mPlayer))
	{
		mPlayer2->GetVelocity()->Set(mPlayer2->GetVelocity()->x *-1, mPlayer2->GetVelocity()->y,mPlayer2->GetVelocity()->z *-1);
	}
		
	if(gCollisionManager->isCollided((Entity*)mPlayer,(Entity*)mBall))
	{
		mBall->GetVelocity()->Set(mBall->GetVelocity()->x + mPlayer->GetVelocity()->x,mPlayer->GetVelocity()->y,mBall->GetVelocity()->z + mPlayer->GetVelocity()->z);
	}

	if(gCollisionManager->isCollided((Entity*)mPlayer2,(Entity*)mBall))
	{
		mBall->GetVelocity()->Set(mBall->GetVelocity()->x + mPlayer2->GetVelocity()->x,mPlayer2->GetVelocity()->y,mBall->GetVelocity()->z + mPlayer2->GetVelocity()->z);
	}

	if(gCollisionManager->isCollided((Entity*)mTeam1Net,(Entity*)mBall))
	{
	}
		
	if(gCollisionManager->isCollided((Entity*)mTeam2Net,(Entity*)mBall))
	{
	}
}

void Game::Draw()
{
	//Still a bug with the texture manager not rendering font image.
	//gTextManager->DrawText("Player 1",100, Justify_CenterX && Justify_CenterY, Vector3(0,0,1), Vector3(0,0,0),gCamera);

	for(int i = 0; i < mEntityList.size(); i++)
	{
		mEntityList.at(i)->Draw();
	}
}
