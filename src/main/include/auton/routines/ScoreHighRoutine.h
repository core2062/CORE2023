#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"
#include "auton/ArmAction.h"
#include "auton/ElevatorAction.h"
#include "auton/ScoreHighAction.h"

using namespace CORE;
using namespace std;

class ScoreHighRoutine: public COREAuton {
public:
    ScoreHighRoutine();
    void AddNodes() override;
private:
    Node * scoreHighNode;
    Node * delayNode1;
    Node * dropNode;
    Node * delayNode2;
    Node * clawCloseNode;
    Node * delayNode3;
    Node * armInNode;
    Node * delayNode4;
    Node * wristNode2;
    Node * delayNode5;
    Node * elevatorDownNode;
};