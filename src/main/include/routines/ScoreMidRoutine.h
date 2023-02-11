#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreMidAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"

using namespace CORE;
using namespace std;

class ScoreMidRoutine: public COREAuton {
public:
    ScoreMidRoutine();
    void AddNodes() override;
private:
    Node * grabNode;
    Node * delayNode;
    Node * scoremidNode;
};