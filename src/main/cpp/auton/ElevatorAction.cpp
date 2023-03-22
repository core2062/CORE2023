#include "auton/ElevatorAction.h"
#include "Robot.h"

ElevatorAction::ElevatorAction(elevatorAction requestedElevatorAction, double requestedElevatorPosition) {
    m_elevatorAction = requestedElevatorAction;
    m_elevatorPosition = requestedElevatorPosition;
}

void ElevatorAction::ActionInit() {
    m_elevatorSubsystem = &Robot::GetInstance()->elevatorSubsystem;
}

CORE::COREAutonAction::actionStatus ElevatorAction::Action() {
    
    switch(m_elevatorAction) {
        case ELEVATOR_UP:
            if (m_elevatorSubsystem->IsElevatorUp()) {
                m_elevatorSubsystem->SetRequestedSpeed(0);
                // Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                std::cout << "elev all the way up" << endl;
                break;
            } else {
                m_elevatorSubsystem->SetRequestedSpeed(.6);
                std::cout << "2" << endl;
                Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
                // Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
        case ELEVATOR_DOWN:
            if (m_elevatorSubsystem->IsElevatorDown()) {
                m_elevatorSubsystem->SetRequestedSpeed(0);
                // Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                break;
            } else {
                m_elevatorSubsystem->SetRequestedSpeed(-.6);
                Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
                // Robot::GetInstance()->elevatorSubsystem.MoveElevator();
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
        case SET_ELEVATOR_HIGHT:
            m_elevatorSubsystem->SetRequestedPosition(m_elevatorPosition);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ElevatorAction::ActionEnd() {
    
}