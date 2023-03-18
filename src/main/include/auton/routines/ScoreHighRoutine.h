#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"
#include "auton/ArmAction.h"
#include "auton/ElevatorAction.h"

using namespace CORE;
using namespace std;

class ScoreHighRoutine: public COREAuton {
public:
    ScoreHighRoutine();
    void AddNodes() override;
private:
    Node * elevatorUpNode;
    Node * delayNode1;
    Node * wristNode;
    Node * delayNode2;
    Node * armOutNode;
    Node * delayNode3;
    Node * dropNode;
    Node * delayNode4;
    Node * clawCloseNode;
    Node * delayNode5;
    Node * armInNode;
    Node * delayNode6;
    Node * wristNode2;
    Node * delayNode7;
    Node * elevatorDownNode;
};