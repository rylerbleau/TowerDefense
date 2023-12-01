#include "Desicion.h"

DesicionTreeNode* Desicion::makeDecision()
{
	DesicionTreeNode* branch = getBranch();
	return branch->makeDecision();
}

bool Desicion::testValue()
{
	return false;
}

DesicionTreeNode* Desicion::getBranch()
{
	if (testValue())
	{
		return trueNode;
	}
	else
	{
		return falseNode;
	}
}

/*
*
* TurretInRange
* 
*/

#include "Character.h"
#include "Turret.h"

bool TurretInRange::testValue()
{
	// find the closest turret
	Turret* closestTurret = nullptr;
	float leastDist = 1000000.0f;
	for (int i = 0; i < turrets->size(); i++) {
		float dist = VMath::distance(player->getBody()->getPos(), (*turrets)[i]->getBody()->getPos());
		if (dist < leastDist) {
			// get the closest target
			leastDist = dist;
			closestTurret = (*turrets)[i];
		}
	}
	if (!closestTurret)
	{
		return false;
	}
	// find the magnitude of the vector from player to turret
	if(VMath::mag(player->getBody()->getPos() - closestTurret->getBody()->getPos()) < 5.0f)
	{
		return true;
	}
	return false;
}


