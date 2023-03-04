// #pragma once

// #include <string>
// #include <CORERobotLib.h>
// #include "auton/DriveAction.h"
// #include "auton/DelayAction.h"
// #include "auton/ScoreHighAction.h"
// #include "auton/GrabberAction.h"
// #include "auton/AutoBalanceAction.h"
// #include "auton/PickupAction.h"

// using namespace CORE;
// using namespace std;

// class HighScoreRoutine: public COREAuton {
// public:
//     HighScoreRoutine();
//     void AddNodes() override;
// private:
//     //Pickup prep
//     Node * wristNode;
//     Node * delayNode;
//     Node * intakeNode;
//     Node * delayNode;
//     Node * autonPickupNode;
//     Node * delayNode2;
//     //pickup and score
//     Node * grabNode;
//     Node * delayNode3;
//     Node * scoreHighNode;
//     Node * delayNode4;
//     Node * dropNode;
//     Node * delayNode5;
//     //prepeare to grab new piece
//     Node * pickupNode;
//     Node * delayNode6;
//     //move and grab new piece
//     Node * turnNode;
//     Node * delayNode5;
//     Node * intakeNode;
//     Node * delayNode7;
//     Node * driveNode;
//     Node * delayNode6;
//     //grab piece
//     Node * grabberNode3;
//     Node * delayNode8;
//     Node * intakeNode2;
//     Node * delayNode9;
//     //return to score
//     Node * turnNode2;
//     Node * delayNode10;
//     Node * driveNode2;
//     Node * delayNode11;
//     Node * scoreMidNode;
//     Node * delayNode12;
//     Node * dropNode;
//     Node * delayNode;
//     Node * pickupNode2;
//     Node * delayNode13;
//     Node * wristNode;
//     Node * delayNode14;
//     Node * intakeNode;
//     Node * delayNode;
//     Node * wristNode;
//     Node * delayNode;
//     //go to balance
//     Node * turnNode3;
//     Node * delayNode15;
//     Node * driveNode3;
//     Node * delayNode16;
//     Node * turnNode4;
//     Node * delayNode17;
//     Node * driveNode4;
//     Node * delayNode18;
//     Node * balanceNode;
// };