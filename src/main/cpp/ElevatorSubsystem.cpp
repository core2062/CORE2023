#include "ElevatorSubsystem.h"

using namespace CORE;

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftMotor(LEFT_LIFT_PORT),
        m_rightMotor(RIGHT_LIFT_PORT), 
        m_liftSpeedModifier("Lift speed Modifier", 0.5){
}


void ElevatorSubsystem::RobotInit(){
        m_leftMotor.Set(ControlMode::PercentOutput, 0);
    m_rightMotor.Set(ControlMode::PercentOutput, 0);

    m_leftMotor.SetNeutralMode(NeutralMode::Brake);
    m_rightMotor.SetNeutralMode(NeutralMode::Brake);

    m_liftStart = m_leftMotor.GetSelectedSensorPosition();
    
}


void ElevatorSubsystem::TeleopInit() {
}

void ElevatorSubsystem::Teleop(){  
}

void ElevatorSubsystem::ElevatorSpeed(double liftSpeed) {
    m_leftMotor.Set(ControlMode::PercentOutput, -liftSpeed);
    m_rightMotor.Set(ControlMode::PercentOutput, liftSpeed);
}