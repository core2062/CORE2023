#include "ElevatorSubsystem.h"

using namespace CORE;

ElevatorSubsystem::ElevatorSubsystem() : 
        m_leftLiftMotor(LEFT_LIFT_PORT),
        m_rightLiftMotor(RIGHT_LIFT_PORT),
        m_bottomLimitSwitch(ELEVATOR_BOTTOM_LIMIT_SWITCH_PORT),
        m_topLimitSwitch(ELEVATOR_TOP_LIMIT_SWITCH_PORT),
        m_pickUpHeight("Elevator Pick Up Height"),
        m_mediumHeight("Elevator Mid-Level Height"),
        m_highHeight("Elevator High-Level Height"),
        m_ticksPerInch("Elevator Ticks Per Inch"),
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

    m_rightLiftMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor,0,0);
    m_rightLiftMotor.SetSelectedSensorPosition(0,0,0);

    m_rightLiftMotor.SetSensorPhase(true);

}

void ElevatorSubsystem::teleopInit(){
    SetRequestedPosition(GetElevatorInches());
    m_rightLiftMotor.ConfigMotionCruiseVelocity(m_cruiseVel.Get(),0);
    m_rightLiftMotor.ConfigMotionAcceleration(m_maxAcel.Get(),0);
}

void ElevatorSubsystem::teleop(){}

void ElevatorSubsystem::PostLoopTask(){}

void ElevatorSubsystem::SetRequestedPosition(double positionInInches){}

void ElevatorSubsystem::SetRequestedSpeed(double speed){}

void ElevatorSubsystem::SetHighHeight(){}

void ElevatorSubsystem::SetMediumHeight(){}

void ElevatorSubsystem::SetPickupHeight(){}

int ElevatorSubsystem::GetElevatorPosition(){}

double ElevatorSubsystem::GetElevatorInches(){}

bool ElevatorSubsystem::LiftDown(){}

bool ElevatorSubsystem::LiftTop(){}

bool ElevatorSubsystem::IsHighHeight(){}

bool ElevatorSubsystem::IsMediumHeight(){}

bool ElevatorSubsystem::IsPickupHeight(){}

void ElevatorSubsystem::ResetEncoder(){
    m_rightLiftMotor.SetSelectedSensorPosition(0,0,0);
}