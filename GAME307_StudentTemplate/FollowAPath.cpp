#include "FollowAPath.h"

FollowAPath::FollowAPath(Body* character_, std::vector<Node*> path_) : 
    
    KinematicArrive{character_, nullptr}
{
    target = new Body{};
    path = new Path(path_);
}

FollowAPath::~FollowAPath()
{
}

KinematicSteeringOutput* FollowAPath::getSteering()
{
    Vec3 target_position;
    if (!path) return nullptr;

    target_position = path->getCurrentNodePositon();

    if (VMath::distance(character->getPos(), target_position) <= character->getRadius())
    {
        path->incrementCurrentNode(1);
    }

    target->setPos(target_position);

    return KinematicArrive::getSteering();

}
