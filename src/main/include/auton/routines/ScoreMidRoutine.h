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

class ScoreMidRoutine: public COREAuton {
public:
    ScoreMidRoutine();
    void AddNodes() override;
private:
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
};