#include "routines/ScoreMidFieldBalanceRoutine.h"

ScoreMidFieldBalanceRoutine::ScoreMidFieldBalanceRoutine() : COREAuton("Score High Routine") {}

void ScoreMidFieldBalanceRoutine::AddNodes() {
    grabNode = new Node(10, new GrabberAction(GRABBER_GRAB));
    delayNode = new Node(10, new DelayAction());
    scoreMidNode = new Node(10, new ScoreMidAction(SCORE_MID));
    delayNode2 = new Node(10, new DelayAction());
    pickupNode = new Node(5, new PickupAction(PICKUP));
    delayNode3 = new Node(10, new DelayAction());
    grabNode2 = new Node(5, new GrabberAction(GRABBER_RELEASE));
    delayNode4 = new Node(10, new DelayAction());
    driveNode = new Node(10, new DriveAction(BACKWARD, 24));
    delayNode5 = new Node(10, new DelayAction());
    driveNode = new Node(10, new DriveAction(FORWARD, 24));
    delayNode6 = new Node(10, new DelayAction());
    BalanceNode = new Node(10, new AutoBalanceAction(AUTO_BALANCE));
    AddFirstNode(grabNode);
    grabNode -> AddNext(delayNode);
    delayNode -> AddNext(scoreMidNode);
    scoreMidNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(pickupNode);
    pickupNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(grabNode2);
    grabNode2 -> AddNext(delayNode4);
    delayNode4 -> AddNext(driveNode);
    driveNode -> AddNext(delayNode5);
    delayNode5 -> AddNext(driveNode2);
    driveNode2 -> AddNext(delayNode6);
    delayNode6 -> AddNext(BalanceNode);
}