#include "auton/IntakeAction.h"
#include "Robot.h"

IntakeAction::IntakeAction(intakeAction requestedIntakeAction) {
    m_intakeAction = requestedIntakeAction;
}

void IntakeAction::ActionInit() {
    m_intakeSpeed = Robot::GetInstance()->intakeSubsystem.m_intakeConeSpeed.Get();
}

CORE::COREAutonAction::actionStatus IntakeAction::Action() {
    switch(m_intakeAction) {
        case INTAKE_IN:
            Robot::GetInstance()->intakeSubsystem.SetIntake(m_intakeSpeed);
            break;
        case INTAKE_OUT:
            Robot::GetInstance()->intakeSubsystem.SetIntake(-m_intakeSpeed);
            break;
        case INTAKE_STOP:
            Robot::GetInstance()->intakeSubsystem.SetIntake(0);
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void IntakeAction::ActionEnd() {
    
}