#include "ElevatorSubsystem.h"
#include "Robot.h"

using namespace CORE;

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftLiftMotor(LEFT_LIFT_PORT),
        m_rightLiftMotor(RIGHT_LIFT_PORT),
        m_bottomLimitSwitch(ELEVATOR_BOTTOM_LIMIT_SWITCH_PORT),
        m_topLimitSwitch(ELEVATOR_TOP_LIMIT_SWITCH_PORT),
        m_pickUpHeight("Elevator Pick Up Height"),
        m_mediumHeight("Elevator Mid-Level Height"),
        m_highHeight("Elevator High-Level Height"),
        m_ticksPerMeter("Elevator Ticks Per Meter"),
        m_bottomLimit("Elevator Bottom Limit"),
        m_topLimit("Elevator Top Limit"),
        m_cruiseVel("Elevator Cruise Velocity"),
        m_maxAcel("Elevator Max Acceleration")
{}

void ElevatorSubsystem::robotInit(){
    // Lines 20-33 config the talons
    m_leftLiftMotor.Set(ControlMode::PercentOutput, 0);
    m_rightLiftMotor.Set(ControlMode::PercentOutput, 0);

    m_leftLiftMotor.SetNeutralMode(NeutralMode::Brake);
    m_rightLiftMotor.SetNeutralMode(NeutralMode::Brake);

    m_rightLiftMotor.SetInverted(true);

    m_leftLiftMotor.Follow(m_rightLiftMotor);

    m_rightLiftMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative,0,0);
    m_rightLiftMotor.SetSelectedSensorPosition(0,0,0);

    m_rightLiftMotor.SetSensorPhase(true);

    operatorJoystick->RegisterAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y);

}

void ElevatorSubsystem::teleopInit(){
    SetRequestedPosition(GetElevatorInches());
    m_rightLiftMotor.ConfigMotionCruiseVelocity(m_cruiseVel.Get(),0);
    m_rightLiftMotor.ConfigMotionAcceleration(m_maxAcel.Get(),0);
}

void ElevatorSubsystem::teleop(){}

void ElevatorSubsystem::PostLoopTask(){
    SmartDashboard::PutNumber("Elevator Position",m_rightLiftMotor.GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Elevator Velocity",m_rightLiftMotor.GetSelectedSensorVelocity(0));
    SmartDashboard::PutNumber("Requested Elevator Position",m_requestedPosition);

    double elevatorPosition = GetElevatorInches();

    SetRequestedSpeed(-operatorJoystick->GetAxis(CORE::COREJoystick::JoystickAxis::RIGHT_STICK_Y));

    SmartDashboard::PutNumber("Elevator Speed",m_requestedSpeed);
    if(m_requestedSpeed < -0.01 || m_requestedSpeed > 0.1)
    {
        if(m_requestedSpeed < 0)
            m_requestedSpeed *= 0.1;
        else
            m_requestedSpeed *= 0.5;
        SetRequestedPosition(elevatorPosition);
        Robot::GetInstance()->scoringAssembly.SetWantedState(WantedState::MANUAL);
    }

    double elevatorRequestedPosition = m_requestedPosition;

    if(m_requestedSpeed > 0 && ElevatorUp())
    {
        m_requestedSpeed = 0;
        SetRequestedPosition(m_topLimit.Get());
    } else if(ElevatorDown())
    {
        if(m_requestedSpeed < 0)
        {
            m_requestedSpeed = 0;
            SetRequestedPosition(0);
        }
        ResetEncoders();
    }

    if (m_requestedSpeed < -0.01 || m_requestedSpeed > 0.1)
    {
        m_rightLiftMotor.Set(ControlMode::PercentOutput, m_requestedSpeed);
    } else
    {
        m_rightLiftMotor.Set(ControlMode::MotionMagic,elevatorRequestedPosition);
    }
    
    m_requestedSpeed = 0;
    SmartDashboard::PutNumber("Elevator",elevatorPosition);
}

void ElevatorSubsystem::SetRequestedPosition(double positionInInches){
    auto position = (int)(positionInInches * m_ticksPerMeter.Get());
    position = max(position,0);
    position = min(position, (int)(m_topLimit.Get()*m_ticksPerMeter.Get()));
    m_requestedPosition = position;
}

void ElevatorSubsystem::SetRequestedSpeed(double speed){
    m_requestedSpeed = speed;
}

void ElevatorSubsystem::SetHighHeight(){
    SetRequestedPosition(m_highHeight.Get());
}

void ElevatorSubsystem::SetMediumHeight(){
    SetRequestedPosition(m_mediumHeight.Get());
}

void ElevatorSubsystem::SetPickupHeight(){
    SetRequestedPosition(m_pickUpHeight.Get());
}

int ElevatorSubsystem::GetElevatorPosition(){
    return m_rightLiftMotor.GetSelectedSensorPosition(0);
}

double ElevatorSubsystem::GetElevatorInches(){
    return GetElevatorPosition() / m_ticksPerMeter.Get();
}

bool ElevatorSubsystem::ElevatorDown(){
    return m_bottomLimitSwitch.Get();
}

bool ElevatorSubsystem::ElevatorUp(){
    return m_topLimitSwitch.Get();
}

bool ElevatorSubsystem::IsHighHeight(){
    return abs(GetElevatorInches() - m_highHeight.Get()) < 2;
}

bool ElevatorSubsystem::IsMediumHeight(){
    return abs(GetElevatorInches() - m_mediumHeight.Get()) < 2;
}

bool ElevatorSubsystem::IsPickupHeight(){
    return abs(GetElevatorInches() - m_pickUpHeight.Get()) < 2;
}

void ElevatorSubsystem::ResetEncoders(){
    m_rightLiftMotor.SetSelectedSensorPosition(0,0,0);
}
