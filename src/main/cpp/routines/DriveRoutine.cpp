#include "routines/DriveRoutine.h"

DriveRoutine::DriveRoutine() : COREAuton("Drive Routine") {}

void DriveRoutine::AddNodes() {
    driveNode = new Node(10, new DriveAction(FORWARD, 36));
    AddFirstNode(driveNode);
}