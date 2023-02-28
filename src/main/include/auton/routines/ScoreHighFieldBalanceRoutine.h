#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"
#include "auton/PickupAction.h"

using namespace CORE;
using namespace std;

class ScoreHighFieldBalanceRoutine: public COREAuton {
public:
    ScoreHighFieldBalanceRoutine();
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
    //return to start pose
    Node * pickupNode;
    Node * delayNode6;
    Node * wristNode2;
    Node * delayNode;
    Node * intakeNode;
    Node * delayNode;
    Node * wristNode;
    Node * delayNode;
    //drive to exit start area
    Node * driveNode;
    Node * delayNode5;
    Node * turnNode;
    Node * delayNode;
    Node * driveNode;
    Node * delayNode;
    Node * turnNode;
    Node * delayNode;
    Node * driveNode2;
    Node * delayNode6;
    Node * BalanceNode;


};