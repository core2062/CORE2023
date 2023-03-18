#pragma once

#include <CORERobotLib.h>
#include "ElevatorSubsystem.h"

using namespace CORE;

enum elevatorAction{
    ELEVATOR_UP,
    ELEVATOR_DOWN,
    SET_ELEVATOR_HIGHT
};

class ElevatorAction : public COREAutonAction {
    public:
        ElevatorAction(elevatorAction requestedElevatorAction, double requestedElevatorPosition);
        void ActionInit() override;
        CORE::COREAutonAction::actionStatus Action() override;
        void ActionEnd() override;
    private:
        elevatorAction m_elevatorAction;
        double m_elevatorPosition;
        ElevatorSubsystem* m_elevatorSubsystem;
};