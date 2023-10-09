#ifndef KINEMATIC_SEPERATION_H
#define KINEMATIC_SEPERATION_H

#include <Vector.h>
#include "VMath.h"
#include "Body.h"
#include "KinematicSteeringOutput.h"
#include <vector>


using namespace MATH;


class KinematicSeperation
{
private:
	KinematicSteeringOutput* result;
	

	
public:
	KinematicSeperation(std::vector<Body*> group);
	KinematicSteeringOutput* GetSteering();
	
	~KinematicSeperation();


};


#endif