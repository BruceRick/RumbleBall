#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

class CollisionManager;

extern CollisionManager* gCollisionManager;

class CollisionManager
{
public:
	CollisionManager();

	~CollisionManager();

	bool isCollided(Entity* pEntity1, Entity* pCollidiedEntity);
};

#endif
