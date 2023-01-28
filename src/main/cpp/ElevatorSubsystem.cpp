#include "ElevatorSubsystem.h"

using namespace CORE;

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftLiftMotor(LEFT_LIFT_PORT),
        m_rightLiftMotor(RIGHT_LIFT_PORT),
        {
}


void ElevatorSubsystem::robotInit(){
    // operatorJoystick->RegisterAxis(CORE::COREJoystick::LEFT_STICK_Y);

    m_leftLiftMotor.Set(ControlMode::PercentOutput, 0);
    m_rightLiftMotor.Set(ControlMode::PercentOutput, 0);

    m_leftLiftMotor.SetNeutralMode(NeutralMode::Brake);
    m_rightLiftMotor.SetNeutralMode(NeutralMode::Brake);
}


void ElevatorSubsystem::teleopInit() {
}

void ElevatorSubsystem::teleop(){  
}