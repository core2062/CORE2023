#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"

using namespace CORE;
using namespace std;

class ScoreHighRoutine: public COREAuton {
public:
    ScoreHighRoutine();
    void AddNodes() override;
private:
    //Pickup prep
    Node * wristNode;
    Node * delayNode;
    Node * intakeNode;
    Node * delayNode;
    Node * autonPickupNode;
    Node * delayNode2;
    //pickup and score
    Node * grabNode;
    Node * delayNode3;
    Node * scoreHighNode;
    Node * delayNode4;
    Node * dropNode;
    Node * delayNode5;
};