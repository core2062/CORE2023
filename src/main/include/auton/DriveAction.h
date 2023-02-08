#pragma once

#include <CORERobotLib.h>
#include "DriveSubsystem.h"

using namespace CORE;

enum driveAction {
    FORWARD,
    BACKWARD,
    RIGHT_TURN,
    LEFT_TURN
};

class DriveAction : public COREAutonAction {
public:
    DriveAction(driveAction requestedDriveAction); // Use for FORWARD and BACKWARD actions
    DriveAction(driveAction requestedDriveAction, int requestedDistance); // Use for FORWARD and BACKWARD actions
    DriveAction(driveAction requestedDriveAction, double turnAmount); // Use for TURN_RIGHT and TURN_LEFT actions
    void ActionInit() override;
    CORE::COREAutonAction::actionStatus Action() override;
    void ActionEnd() override;
private:
    driveAction m_driveAction;
    double m_encoderStartUpPosition, m_encoderValue, 
            m_navXStartingHeading, m_requestedHeading, m_currentHeading,
            m_turnAmount, m_requestedDriveDistance;
    COREConstant<double> m_distAutonMoveEncoderTicks;
};