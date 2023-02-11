#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreMidAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"

using namespace CORE;
using namespace std;

class MidScoreRoutine: public COREAuton {
public:
    MidScoreRoutine();
    void AddNodes() override;
private:
    Node * grab;
    Node * delay;
    Node * scoremid;
};