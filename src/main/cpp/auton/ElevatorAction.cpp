#include "auton/ElevatorAction.h"
#include "Robot.h"

ElevatorAction::ElevatorAction(elevatorAction requestedElevatorAction, double requestedElevatorPosition) {
    m_elevatorAction = requestedElevatorAction;
    m_elevatorPosition = requestedElevatorPosition;
}

void ElevatorAction::ActionInit() {
    
}

CORE::COREAutonAction::actionStatus ElevatorAction::Action() {
    switch(m_elevatorAction) {
        case ELEVATOR_UP:
            if (Robot::GetInstance()->elevatorSubsystem.IsElevatorUp()) {
                Robot::GetInstance()->elevatorSubsystem.SetRequestedSpeed(0);
                Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                break;
            } else {
                Robot::GetInstance()->elevatorSubsystem.SetRequestedSpeed(.6);
                Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
        case ELEVATOR_DOWN:
            if (Robot::GetInstance()->elevatorSubsystem.IsElevatorDown()) {
                Robot::GetInstance()->elevatorSubsystem.SetRequestedSpeed(0);
                Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                break;
            } else {
                Robot::GetInstance()->elevatorSubsystem.SetRequestedSpeed(-.6);
                Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
        case SET_ELEVATOR_HIGHT:
            Robot::GetInstance()->elevatorSubsystem.SetRequestedPosition(m_elevatorPosition);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ElevatorAction::ActionEnd() {
    
}