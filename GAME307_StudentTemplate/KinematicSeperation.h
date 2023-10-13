#ifndef KINEMATIC_SEPERATION_H
#define KINEMATIC_SEPERATION_H

#include <Vector.h>
#include "VMath.h"
#include "Body.h"
#include "KinematicSteeringOutput.h"
#include "StaticBody.h"
#include <vector>


using namespace MATH;


class KinematicSeperation
{
private:
	KinematicSteeringOutput* result;

	Body* character;
	std::vector<StaticBody*> bodies;
	float threshold;
	int myIndex;


	

	
public:
	

	// for some reason, can't use polymorphism to pass bodies into this constructor
	KinematicSeperation(Body* character_, std::vector<StaticBody*> &bodies_, float threshold_, int myIndex_);

	KinematicSteeringOutput* GetSteering();
	
	~KinematicSeperation();


};


#endif