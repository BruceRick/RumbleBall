#ifndef _PHYSICSMANGAGER_H_
#define _PHYSICSMANAGER_H_


class PhysicsManager
{
public:
	PhysicsManager(std::vector<Entity*> pEntityList, PlayingFieldBounds pPlayingField);
	~PhysicsManager();

	void ApplyPhysics(double p_ElapsedTime);
private:

	std::vector<Entity*> mEntityList;
	float mFriction;
	PlayingFieldBounds mPlayingField;
};

#endif



