#include "auton/routines/DriveRoutine.h"

DriveRoutine::DriveRoutine() : COREAuton("Drive Routine") {}

void DriveRoutine::AddNodes() {
    driveNode = new Node(5, new DriveAction(FORWARD, 65));
    delayNode = new Node(5, new DelayAction());
    driveNode2 = new Node(5, new DriveAction(RIGHT_TURN, 90));
    delayNode2 = new Node(3, new DelayAction());
    driveNode3 = new Node(5, new DriveAction(FORWARD, 36));
    delayNode3 = new Node(5, new DelayAction());
    driveNode4 = new Node(5, new DriveAction(RIGHT_TURN, 90));
    delayNode4 = new Node(3, new DelayAction());
    driveNode5 = new Node(5, new DriveAction(FORWARD, 36));
    delayNode5 = new Node(3, new DelayAction());
    balanceNode = new Node(5, new AutoBalanceAction(AUTO_BALANCE, 5));
    AddFirstNode(driveNode);
    driveNode -> AddNext(delayNode);
    delayNode -> AddNext(driveNode2);
    driveNode2 -> AddNext(delayNode2);
    delayNode2 -> AddNext(driveNode3);
    driveNode3 -> AddNext(delayNode3);
    delayNode3 -> AddNext(driveNode4);
    driveNode4 -> AddNext(delayNode4);
    delayNode4 -> AddNext(driveNode5);
    driveNode5 -> AddNext(delayNode5);
    delayNode5 -> AddNext(balanceNode);
}