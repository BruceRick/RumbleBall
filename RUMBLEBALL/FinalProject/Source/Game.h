#ifndef __GAME_H__
#define __GAME_H__



class Game : Screen
{
public:
	Game();
	~Game();

	void Initialize();
	
	void Update(double pElapsedTime);
	void Draw();

private:
	
	Player* mPlayer;
	Player* mPlayer2;
	Ball* mBall;
	Entity* mStadium;
	Entity* mSkySphere;
	
	Net* mTeam1Net;
	Net* mTeam2Net;

	float mCameraDistance;
	PhysicsManager* mPhysicsManager;
	std::vector<Entity*> mEntityList;


};

#endif

