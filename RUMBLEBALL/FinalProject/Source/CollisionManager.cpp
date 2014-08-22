#include "CommonHeader.h";

CollisionManager* gCollisionManager = 0;

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::isCollided(Entity* pEntity, Entity* pCollidedEntity)
{
	/*if(pEntity->mPosition->x  + pEntity->GetModel()->m_Size->Width  >= pCollidedEntity->mPosition->x &&
		pEntity->mPosition->x + pEntity->GetModel()->m_Size->Width  <= pCollidedEntity->mPosition->x + pCollidedEntity->GetModel()->m_Size->Width &&
		pEntity->mPosition->z >= pCollidedEntity->mPosition->z &&
		pEntity->mPosition->z <= pCollidedEntity->mPosition->z + pCollidedEntity->GetModel()->m_Size->Length)
	{
		return true;
	}*/
	float x = pEntity->mPosition->x - pCollidedEntity->mPosition->x;
	float z = pEntity->mPosition->z - pCollidedEntity->mPosition->z;

	float width = pEntity->GetModel()->m_Size->Width;
	float length =pEntity->GetModel()->m_Size->Length;

	if(pEntity->mPosition->x - pCollidedEntity->mPosition->x < pEntity->GetModel()->m_Size->Width && pEntity->mPosition->x - pCollidedEntity->mPosition->x > pEntity->GetModel()->m_Size->Width * -1 && 
	   pEntity->mPosition->z - pCollidedEntity->mPosition->z < pEntity->GetModel()->m_Size->Length && pEntity->mPosition->z - pCollidedEntity->mPosition->z > pEntity->GetModel()->m_Size->Length * -1) 
	{
		return true;
	}
	else
	{
		return false;
	}
}
