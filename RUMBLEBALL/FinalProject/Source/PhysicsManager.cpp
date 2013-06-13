#include "CommonHeader.h"


PhysicsManager::PhysicsManager(std::vector<Entity*> pEntityList, PlayingFieldBounds pPlayingField)
{

	mEntityList = pEntityList;
	mFriction = 0.5;
	mPlayingField = pPlayingField;
}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::ApplyPhysics(double p_ElapsedTime)
{
	for(int i = 0; i < mEntityList.size(); i++)
	{
		if(mEntityList.at(i)->mVelocity->x != -1)
		{
			float friction = mFriction * p_ElapsedTime;
			
			

			//ErrorLog((std::string)friction);
			//pow(mFriction,(float)p_ElapsedTime);
			
			mEntityList.at(i)->mVelocity->x *= pow(mFriction,(float)p_ElapsedTime);
			mEntityList.at(i)->mVelocity->z *= pow(mFriction,(float)p_ElapsedTime);
			if(mEntityList.at(i)->mPosition->x > mPlayingField.MaxX)
			{
				mEntityList.at(i)->mPosition->x = mPlayingField.MaxX;
				mEntityList.at(i)->mVelocity->x *= -1;
			}

			if(mEntityList.at(i)->mPosition->x < mPlayingField.MinX)
			{
				mEntityList.at(i)->mPosition->x = mPlayingField.MinX;
				mEntityList.at(i)->mVelocity->x *= -1;
			}

			if(mEntityList.at(i)->mPosition->z < mPlayingField.MinZ)
			{
				mEntityList.at(i)->mPosition->z = mPlayingField.MinZ;
				mEntityList.at(i)->mVelocity->z *= -1;
			}

			if(mEntityList.at(i)->mPosition->z > mPlayingField.MaxZ)
			{
				mEntityList.at(i)->mPosition->z = mPlayingField.MaxZ;
				mEntityList.at(i)->mVelocity->z *= -1;
			}

		}

		
		

	}
	
}