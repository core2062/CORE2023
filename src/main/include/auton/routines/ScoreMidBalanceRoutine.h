#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"
#include "auton/ArmAction.h"

using namespace CORE;
using namespace std;

class ScoreMidBalanceRoutine: public COREAuton {
public:
    ScoreMidBalanceRoutine();
    void AddNodes() override;
private:
    //Score
    Node * wristNode;
    Node * delayNode;
    Node * armOutNode;
    Node * delayNode2;
    Node * dropNode;
    Node * delayNode3;
    Node * clawCloseNode;
    Node * delayNode4;
    Node * armInNode;
    Node * delayNode5;
    Node * wristNode2;
    Node * delayNode6;
    //move to balance
    Node * driveNode;
    Node * delayNode7;
    Node * balanceNode;
};