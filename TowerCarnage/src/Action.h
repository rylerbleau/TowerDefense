#pragma once
#include "DesicionTreeNode.h"

enum class ACTION_SET {
    ARRIVE,
    FIND_PATH,
    DO_NOTHING
};
class Action :
    public DesicionTreeNode
{
public:
    Action() :
       label(ACTION_SET::DO_NOTHING)
    {}
    Action(ACTION_SET label_) : label(label_) {}
    ~Action() {}

    DesicionTreeNode* makeDecision() { return this; }
    ACTION_SET getLabel() const { return label; }
private: 
    ACTION_SET label;
};

