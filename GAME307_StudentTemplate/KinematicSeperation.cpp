#include "KinematicSeperation.h"

KinematicSeperation::KinematicSeperation(std::vector<Body*> group)
{
	result = new KinematicSteeringOutput();


}

KinematicSteeringOutput* KinematicSeperation::GetSteering()
{
	


	return new KinematicSteeringOutput();
}

KinematicSeperation::~KinematicSeperation()
{
}
