#pragma once
#include "KinematicArrive.h"
#include "Path.h"

class FollowAPath :
    public KinematicArrive
{
private:
	Path* path;
public:
	FollowAPath(Body* character_, Path* path_);
	~FollowAPath();
	KinematicSteeringOutput* getSteering();

};

