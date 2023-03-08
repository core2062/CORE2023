#include "auton/routines/ScoreMidRoutine.h"

ScoreMidRoutine::ScoreMidRoutine() : COREAuton("Mid Score Routine") {}

void ScoreMidRoutine::AddNodes() {
    wristNode = new Node(1, new ArmAction(WRIST_UP));
    delayNode = new Node(0, new DelayAction());
    armOutNode = new Node(2, new ArmAction(ARM_OUT_MID));
    delayNode2 = new Node(0.2, new DelayAction());
    dropNode = new Node(1, new GrabberAction(GRABBER_RELEASE));
    delayNode3 = new Node(1, new DelayAction());
    clawCloseNode = new Node(1, new GrabberAction(GRABBER_GRAB));
    delayNode4 = new Node(0, new DelayAction());
    armInNode = new Node(2, new ArmAction(ARM_IN));
    delayNode5 = new Node(0, new DelayAction());
    wristNode2 = new Node(1, new ArmAction(WRIST_DOWN));

    AddFirstNode(wristNode);
    wristNode -> AddNext(delayNode);
    delayNode -> AddNext(armOutNode);
    armOutNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(dropNode);
    dropNode -> AddNext(delayNode3);
    delayNode3 -> AddNext(clawCloseNode);
    clawCloseNode -> AddNext(delayNode4);
    delayNode4 -> AddNext(armInNode);
    armInNode -> AddNext(delayNode5);
    delayNode5 -> AddNext(wristNode2);
}