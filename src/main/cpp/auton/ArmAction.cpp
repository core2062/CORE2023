#include "auton/ArmAction.h"
#include "Robot.h"

ArmAction::ArmAction(armAction requestedArmAction) {
    m_armAction = requestedArmAction;
    m_armSubsystem = &Robot::GetInstance()->armSubsystem;
}

void ArmAction::ActionInit() {

}

CORE::COREAutonAction::actionStatus ArmAction::Action() {
    switch(m_armAction) {
        case WRIST_UP:
            m_armSubsystem->SetWristUp();
            break;
        case WRIST_DOWN:
            m_armSubsystem->SetWristDown();
            break;
        case ARM_OUT_MID: 
            if (m_armSubsystem->IsArmOut()) {
                cout << "Stopping Movement" << endl;
                m_armSubsystem->SetDistRequestedSpeed(0.0);
                break;
            } else {
                m_armSubsystem->SetDistRequestedSpeed(0.6);
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
        case ARM_IN:
            if (m_armSubsystem->IsArmIn()) {
                cout << "Stopping Movement" << endl;
                m_armSubsystem->SetDistRequestedSpeed(0.0);
                break;
            } else {
                m_armSubsystem->SetDistRequestedSpeed(-0.7);
                return COREAutonAction::actionStatus::CONTINUE;
            }
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void ArmAction::ActionEnd() {
    
}