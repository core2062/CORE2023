#pragma once

#include <string>
#include <CORERobotLib.h>
#include "auton/ScoreHighAction.h"
#include "auton/GrabberAction.h"
#include "auton/DelayAction.h"

using namespace CORE;
using namespace std;

class ScoreHighRoutine: public COREAuton {
public:
    ScoreHighRoutine();
    void AddNodes() override;
private:
    Node * grabNode;
    Node * delayNode;
    Node * scorehighNode;
};