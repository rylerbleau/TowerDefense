#pragma once
#include "DesicionTreeNode.h"
#include <vector>
class Desicion :
    public DesicionTreeNode
{
    public:
	Desicion() {}
	Desicion(DesicionTreeNode* trueNode_, DesicionTreeNode* falseNode_) :
		trueNode(trueNode_), falseNode(falseNode_) {}
	~Desicion() {}

	DesicionTreeNode* makeDecision();
	virtual bool testValue();
	DesicionTreeNode* getBranch();
private:
	DesicionTreeNode* trueNode;
	DesicionTreeNode* falseNode;
};

class Turret;
class Character;

class TurretInRange:
		public Desicion
{
	public:
	TurretInRange() {}
	TurretInRange(DesicionTreeNode* trueNode_,
		DesicionTreeNode* falseNode_, 
		Character* player_,
		std::vector<Turret*>* turrets_,
		Turret** closestTurret_) :
		Desicion(trueNode_, falseNode_), player(player_), turrets(turrets_), closestTurret(closestTurret_) {}

	bool testValue() override;
private: 
	std::vector<Turret*>* turrets;
	Turret** closestTurret;
	Character* player;
};
