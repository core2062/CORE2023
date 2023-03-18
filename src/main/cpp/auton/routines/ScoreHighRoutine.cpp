#include "auton/routines/ScoreHighRoutine.h"

ScoreHighRoutine::ScoreHighRoutine() : COREAuton("Score High Routine") {}

void ScoreHighRoutine::AddNodes() {
    elevatorUpNode = new Node(1, new ElevatorAction(ELEVATOR_UP, 0));
    delayNode1 = new Node(1, new DelayAction());
    wristNode = new Node(1, new ArmAction(WRIST_UP));
    delayNode2 = new Node(1, new DelayAction());
    armOutNode = new Node(2, new ArmAction(ARM_OUT_MID));
    delayNode3 = new Node(0.2, new DelayAction());
    dropNode = new Node(1, new GrabberAction(GRABBER_RELEASE));
    delayNode4 = new Node(1, new DelayAction());
    clawCloseNode = new Node(1, new GrabberAction(GRABBER_GRAB));
    delayNode5 = new Node(1, new DelayAction());
    armInNode = new Node(2, new ArmAction(ARM_IN));
    delayNode6 = new Node(1, new DelayAction());
    wristNode2 = new Node(1, new ArmAction(WRIST_DOWN));
    delayNode7 = new Node(1, new DelayAction());
    elevatorDownNode = new Node(1, new ElevatorAction(ELEVATOR_DOWN, 0));

    AddFirstNode(elevatorUpNode);
    elevatorUpNode -> AddNext(delayNode1);
    delayNode1 -> AddNext(wristNode);
    wristNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(armOutNode);
    armOutNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(dropNode);
    dropNode -> AddNext(delayNode4);
    delayNode4 -> AddNext(clawCloseNode);
    clawCloseNode -> AddNext(delayNode5);
    delayNode5 -> AddNext(armInNode);
    armInNode -> AddNext(delayNode6);
    delayNode6 -> AddNext(wristNode2);
    wristNode2 -> AddNext(delayNode7);
    delayNode7 -> AddNext(elevatorDownNode);
}