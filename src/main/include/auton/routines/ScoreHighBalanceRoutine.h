#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"

using namespace CORE;
using namespace std;

class ScoreHighBalanceRoutine: public COREAuton {
public:
    ScoreHighBalanceRoutine();
    void AddNodes() override;
private:
    //Pickup prep
    // Node * wristNode;
    // Node * delayNode;
    // Node * intakeNode;
    // Node * delayNode;
    // Node * autonPickupNode;
    // Node * delayNode2;
    // //pickup and score
    // Node * grabNode;
    // Node * delayNode3;
    // Node * scoreHighNode;
    // Node * delayNode4;
    // Node * dropNode;
    // Node * delayNode5;
    // //return to start pose
    // Node * pickupNode;
    // Node * delayNode6;
    // Node * wristNode2;
    // Node * delayNode;
    // Node * intakeNode;
    // Node * delayNode;
    // Node * wristNode;
    // Node * delayNode;
    // //move to balance
    // Node * driveNode;
    // Node * delayNode7;
    // Node * balanceNode;
};