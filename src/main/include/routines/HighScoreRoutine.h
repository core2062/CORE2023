#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"

using namespace CORE;
using namespace std;

class HighScoreRoutine: public COREAuton {
public:
    HighScoreRoutine();
    void AddNodes() override;
private:
    Node * grab;
    Node * delay;
    Node * scorehigh;
};