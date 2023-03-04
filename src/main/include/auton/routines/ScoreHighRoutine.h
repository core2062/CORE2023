#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"
#include "auton/PickupAction.h"
#include "auton/DriveAction.h"

using namespace CORE;
using namespace std;

class ScoreHighRoutine: public COREAuton {
public:
    ScoreHighRoutine();
    void AddNodes() override;
private:
    Node * pickupPositionNode;
    Node * delayNode;
    Node * grabNode;
    Node * delayNode2;
    Node * scoreHighNode;
    Node * delayNode3;
    Node * releaseNode;
    Node * delayNode4;
    Node * driveNode;
};