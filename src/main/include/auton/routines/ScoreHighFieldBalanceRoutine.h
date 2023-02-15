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
    Node * grabNode;
    Node * delayNode;
    Node * scoreHighNode;
    Node * delayNode2;
    Node * pickupNode;
    Node * delayNode3;
    Node * grabNode2;
    Node * delayNode4;
    Node * driveNode;
    Node * delayNode5;
    Node * driveNode2;
    Node * delayNode6;
    Node * BalanceNode;


};