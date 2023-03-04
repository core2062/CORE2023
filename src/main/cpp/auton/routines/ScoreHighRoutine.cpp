#include "auton/routines/ScoreHighRoutine.h"

ScoreHighRoutine::ScoreHighRoutine() : COREAuton("High Score Routine") {}

void ScoreHighRoutine::AddNodes() {
    pickupPositionNode = new Node(10, new PickupAction(PICKUP));
    delayNode = new Node(5, new DelayAction());
    grabNode = new Node(10, new GrabberAction(GRABBER_GRAB));
    delayNode2 = new Node(5, new DelayAction());
    scoreHighNode = new Node(10, new ScoreHighAction(SCORE_HIGH));
    delayNode3 = new Node(5, new DelayAction());
    releaseNode = new Node(10, new GrabberAction(GRABBER_RELEASE));
    delayNode4 = new Node(5, new DelayAction());
    driveNode = new Node(10, new DriveAction(BACKWARD, 36));

    AddFirstNode(pickupPositionNode);
    pickupPositionNode -> AddNext(delayNode);
    delayNode -> AddNext(grabNode);
    grabNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(scoreHighNode);
    scoreHighNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(releaseNode);
    releaseNode -> AddNext(delayNode4);
    delayNode4 -> AddNext(driveNode);
}