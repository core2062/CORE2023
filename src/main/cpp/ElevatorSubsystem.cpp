#include "ElevatorSubsystem.h"

using namespace CORE;

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftMotor(LEFT_LIFT_PORT),
        m_rightMotor(RIGHT_LIFT_PORT) {
}


void ElevatorSubsystem::RobotInit(){
        m_leftMotor.Set(ControlMode::PercentOutput, 0);
    m_rightMotor.Set(ControlMode::PercentOutput, 0);

    m_leftMotor.SetNeutralMode(NeutralMode::Brake);
    m_rightMotor.SetNeutralMode(NeutralMode::Brake);

    m_leftMotor.Follow(m_rightMotor);

    m_rightMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 0);
    m_rightMotor.SetSelectedSensorPosition(0, 0, 0);

    m_rightMotor.SetSensorPhase(true);
}


void ElevatorSubsystem::TeleopInit() {
}

void ElevatorSubsystem::Teleop(){   
}