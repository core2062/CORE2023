#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/DriveAction.h"
#include "auton/DelayAction.h"
#include "auton/AutoBalanceAction.h"

using namespace CORE;
using namespace std;

class DriveRoutine: public COREAuton {
public:
    DriveRoutine();
    void AddNodes() override;
private:
    Node * driveNode;
    Node * delayNode;
};