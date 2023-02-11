#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/ScoreMidAction.h"
#include "auton/GrabberAction.h"
#include "auton/AutoBalanceAction.h"

using namespace CORE;
using namespace std;

class ScoreMidBalanceRoutine: public COREAuton {
public:
    ScoreMidBalanceRoutine();
    void AddNodes() override;
private:
    Node * grabNode;
    Node * delayNode;
    Node * scoreMidNode;
    Node * delayNode2;
    Node * driveNode;
    Node * delayNode3;
    Node * balanceNode;
};