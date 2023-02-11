#include "routines/ScoreHighBalanceRoutine.h"

ScoreHighBalanceRoutine::ScoreHighBalanceRoutine() : COREAuton("Score High Routine") {}

void ScoreHighBalanceRoutine::AddNodes() {
    grabNode = new Node(10, new GrabberAction(GRABBER_GRAB));
    delayNode = new Node(10, new DelayAction());
    scoreHighNode = new Node(10, new ScoreHighAction(SCORE_HIGH));
    delayNode2 = new Node(10, new DelayAction());
    driveNode = new Node(10, new DriveAction(BACKWARD, 24));
    delayNode3 = new Node(10, new DelayAction());
    autoBalancerNode = new Node(10, new AutoBalanceAction(AUTO_BALANCE));
    AddFirstNode(grabNode);
}