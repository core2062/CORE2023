#include "auton/routines/DriveRoutine.h"

DriveRoutine::DriveRoutine() : COREAuton("Drive Routine") {}

void DriveRoutine::AddNodes() {
    driveNode = new Node(5, new DriveAction(FORWARD, 65));
    delayNode = new Node(5, new DelayAction());
    driveNode2 = new Node(5, new DriveAction(FORWARD, 36));
    delayNode2 = new Node(3, new DelayAction());
    balanceNode = new Node(5, new AutoBalanceAction(AUTO_BALANCE, 5));
    AddFirstNode(driveNode);
    driveNode -> AddNext(delayNode);
    delayNode -> AddNext(driveNode2);
    driveNode -> AddNext(delayNode2);
    delayNode2 -> AddNext(balanceNode);
}