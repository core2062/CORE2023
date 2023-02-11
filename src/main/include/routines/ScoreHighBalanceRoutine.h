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
    Node * grabNode;
    Node * delayNode;
    Node * scoreHighNode;
    Node * delayNode2;
    Node * driveNode;
    Node * delayNode3;
    Node * balanceNode;
};