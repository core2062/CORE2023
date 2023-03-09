#include "auton/routines/DriveRoutine.h"

DriveRoutine::DriveRoutine() : COREAuton("Drive Routine") {}

void DriveRoutine::AddNodes() {
    driveNode = new Node(5, new DriveAction(FORWARD, 65));
    delayNode = new Node(5, new DelayAction());
    AddFirstNode(driveNode);
    driveNode -> AddNext(delayNode);
}