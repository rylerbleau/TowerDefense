#include "KinematicSeperation.h"

KinematicSeperation::KinematicSeperation(Body* character_, std::vector<StaticBody*> &bodies_, float threshold_, int myIndex_)
{

	character = character_;
	bodies = bodies_;
	threshold = threshold_;
	myIndex = myIndex_;

	

	result = new KinematicSteeringOutput();



}

KinematicSteeringOutput* KinematicSeperation::GetSteering()
{
	// reset rot and vel just in case
	result->rotation = 0.0f;
	result->velocity = Vec3();

	Vec3 dir;

	for (int i = 0; i < bodies.size(); i++) {

		Vec3 theirPos = bodies[i]->getPos();
		Vec3 myPos = character->getPos();

		if (i != myIndex && abs(VMath::distance(theirPos, myPos)) <= threshold) {
			// is within threshold distance

			dir += myPos - theirPos;
		}
		

	}
	if (VMath::mag(dir) != 0.0f) {

		result->velocity = VMath::normalize(dir);
	}

	return result;
}

KinematicSeperation::~KinematicSeperation()
{
}
