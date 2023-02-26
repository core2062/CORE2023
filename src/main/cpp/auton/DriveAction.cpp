#include "auton/DriveAction.h"
#include "Robot.h"

DriveAction::DriveAction(driveAction requestedDriveAction) : 
                                        m_distAutonMoveEncoderTicks("Auton Movement", 24.0) {
                                        m_driveAction = requestedDriveAction;
                                        m_turnAmount = 0;
                                        m_requestedDriveDistance = (24 *33540)/(6*3.14159265358979323);
}

DriveAction::DriveAction(driveAction requestedDriveAction, int requestedEncoderDistance) :
                                         m_distAutonMoveEncoderTicks("Auton Movement", 24.0) {
                                         m_driveAction = requestedDriveAction;
                                         m_turnAmount = 0;
                                         m_requestedDriveDistance = (requestedEncoderDistance *33540)/(6*3.14159265358979323);
}

DriveAction::DriveAction(driveAction requestedDriveAction, double turnAmount) : 
                                        m_distAutonMoveEncoderTicks("Auton Movement", 24.0) {
                                        m_driveAction = requestedDriveAction;
                                        m_turnAmount = turnAmount;
}

void DriveAction::ActionInit() {
    DriveSubsystem* driveSubsystem = &Robot::GetInstance()->driveSubsystem;
    driveSubsystem->InitTalons();
    driveSubsystem->SetTalonMode(NeutralMode::Brake);
    std::cout << "Resetting Encoder in ActionInit()" << endl;
    // driveSubsystem->resetEncoder();
    // m_requestedDriveDistance = ((m_distAutonMoveEncoderTicks.Get()*33540)/(6*3.14159265358979323));
    m_encoderStartUpPosition =  driveSubsystem->getRobotPosition();
    std::cout << m_requestedDriveDistance << " Requested tick distance" << endl; // should be zero
    std::cout << m_encoderStartUpPosition << " right encoder at startup" << endl; // should be zero
    m_navXStartingHeading = driveSubsystem->ahrs.GetFusedHeading(); //Starting heading of NavX; Used for TURN_RIGHT and TURN_LEFT
}

CORE::COREAutonAction::actionStatus DriveAction::Action() {
    DriveSubsystem* driveSubsystem = &Robot::GetInstance()->driveSubsystem;
    m_encoderValue = driveSubsystem->getRobotPosition();
    switch(m_driveAction) {
        case FORWARD:
            cout <<"Requested Drive Distance " << m_requestedDriveDistance << ", ";
            cout <<"Encoder Value " << m_encoderValue << ", ";
            cout <<"Encoder start up position " << m_encoderStartUpPosition << endl;    
            if (abs(m_encoderValue - m_encoderStartUpPosition) < m_requestedDriveDistance) {
                driveSubsystem->setMotorSpeed(0.4, DriveSide::BOTH);
                return COREAutonAction::actionStatus::CONTINUE;
            } else {
                cout << "Stopping forward" << endl;
                driveSubsystem->setMotorSpeed(0.0, DriveSide::BOTH);
            }
            break;
        case BACKWARD:
            cout << "Encoder Value: "    << m_encoderValue << ", ";
            cout << "Start position: "   << m_encoderStartUpPosition << ", "; 
            cout << "Movement setting: " << m_requestedDriveDistance << endl;
            if (abs(m_encoderValue - m_encoderStartUpPosition) < m_requestedDriveDistance) {
                driveSubsystem->setMotorSpeed(-0.4, DriveSide::BOTH);
                return COREAutonAction::actionStatus::CONTINUE;
            } else {
                cout << "Stopping backup" << endl;
                driveSubsystem->setMotorSpeed(0.0, DriveSide::BOTH);
            }
            break;
        case RIGHT_TURN:
            m_requestedHeading = m_navXStartingHeading + m_turnAmount; // Calculates the requested heading to turn to
            if (m_requestedHeading >= 360) { // If Requested heading is not in a possible range of movement, subtracts 360 to loop it back between 0-359
                m_requestedHeading -= 360;
            } 
            m_currentHeading = driveSubsystem->ahrs.GetFusedHeading();
            if (m_currentHeading != (m_requestedHeading-5) || m_currentHeading != (m_requestedHeading+5)) { // Deadband of 5°
                driveSubsystem->setMotorSpeed(0.2, DriveSide::LEFT);
                driveSubsystem->setMotorSpeed(-0.2, DriveSide::RIGHT);
                return COREAutonAction::actionStatus::CONTINUE;
            } else {
                driveSubsystem->setMotorSpeed(0.0, DriveSide::BOTH);
            }
            break;
        case LEFT_TURN:
            m_requestedHeading = m_navXStartingHeading - m_turnAmount; // Calculates the requested heading to turn to
            if (m_requestedHeading < 0) { // If Requested heading is not in a possible range of movement, adds 360 to loop it back between 0-359
                m_requestedHeading += 360;
            } 
            m_currentHeading = driveSubsystem->ahrs.GetFusedHeading();
            if (m_currentHeading != (m_requestedHeading-5) || m_currentHeading != (m_requestedHeading+5)) { // Deadband of 5°
                driveSubsystem->setMotorSpeed(-0.4, DriveSide::LEFT);
                driveSubsystem->setMotorSpeed(0.4, DriveSide::RIGHT);
                return COREAutonAction::actionStatus::CONTINUE;
            } else {
                driveSubsystem->setMotorSpeed(0.0, DriveSide::BOTH);
            }
            break;
    }
    return COREAutonAction::actionStatus::END;
}

void DriveAction::ActionEnd() {
    DriveSubsystem* driveSubsystem = &Robot::GetInstance()->driveSubsystem;
    std::cout << driveSubsystem->getRobotPosition() << " Right encoder at end" << endl; // should be zero
}