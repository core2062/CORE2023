#include "routines/ScoreMidBalanceRoutine.h"

ScoreMidBalanceRoutine::ScoreMidBalanceRoutine() : COREAuton("Score Mid Routine") {}

void ScoreMidBalanceRoutine::AddNodes() {
    grabNode = new Node(10, new GrabberAction(GRABBER_GRAB));
    delayNode = new Node(10, new DelayAction());
    scoreMidNode = new Node(10, new ScoreMidAction(SCORE_MID));
    delayNode2 = new Node(10, new DelayAction());
    driveNode = new Node(10, new DriveAction(BACKWARD, 24));
    delayNode3 = new Node(10, new DelayAction());
    balanceNode = new Node(10, new AutoBalanceAction(AUTO_BALANCE));
    AddFirstNode(grabNode);
    grabNode -> AddNext(delayNode);
    delayNode -> AddNext(scoreMidNode);
    scoreMidNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(driveNode);
    driveNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(balanceNode);
}