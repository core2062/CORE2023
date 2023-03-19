#include "auton/routines/ScoreHighRoutine.h"

ScoreHighRoutine::ScoreHighRoutine() : COREAuton("Score High Routine") {}

void ScoreHighRoutine::AddNodes() {
    scoreHighNode = new Node(4, new ScoreHighAction());
    delayNode1 = new Node(1, new DelayAction());
    dropNode = new Node(1, new GrabberAction(GRABBER_RELEASE));
    delayNode2 = new Node(1, new DelayAction());
    clawCloseNode = new Node(1, new GrabberAction(GRABBER_GRAB));
    delayNode3 = new Node(1, new DelayAction());
    armInNode = new Node(2, new ArmAction(ARM_IN));
    delayNode4 = new Node(1, new DelayAction());
    wristNode2 = new Node(1, new ArmAction(WRIST_DOWN));
    delayNode5 = new Node(1, new DelayAction());
    elevatorDownNode = new Node(5, new ElevatorAction(ELEVATOR_DOWN, 0));

    AddFirstNode(scoreHighNode);
    scoreHighNode -> AddNext(delayNode1);
    delayNode1 -> AddNext(dropNode);
    dropNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(clawCloseNode);
    clawCloseNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(armInNode);
    armInNode -> AddNext(delayNode4);
    delayNode4 -> AddNext(wristNode2);
    wristNode2 -> AddNext(delayNode5);
    delayNode5 -> AddNext(elevatorDownNode);
}